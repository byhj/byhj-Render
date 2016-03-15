#include "DxutHelper.h"

std::shared_ptr<byhj::DXUTHelper> byhj::DXUTHelper::pDXUTHelper;

//--------------------------------------------------------------------------------------
// UI control IDs
//--------------------------------------------------------------------------------------
#define IDC_TOGGLEFULLSCREEN    1
#define IDC_TOGGLEREF           2
#define IDC_CHANGEDEVICE        3
#define IDC_TOGGLEWARP          4
#define IDC_TOGGLESPIN          5
#define IDC_PUFF_SCALE          6
#define IDC_PUFF_STATIC         7

#include "DXUT.h"

namespace byhj
{

DXUTHelper::DXUTHelper()
{

}

DXUTHelper::~DXUTHelper()
{

}

void DXUTHelper::init()
{
	pDXUTHelper = std::make_shared<DXUTHelper>(*this);
	m_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75f;
	m_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75f;
}

void DXUTHelper::render()
{
	// Set general DXUT callbacks
	DXUTSetCallbackFrameMove(OnFrameMove);
	DXUTSetCallbackKeyboard(OnKeyboard);
	DXUTSetCallbackMouse(OnMouse);
	DXUTSetCallbackMsgProc(MsgProc);
	DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
	DXUTSetCallbackDeviceRemoved(OnDeviceRemoved);

	DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable);
	DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice);
	DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
	DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
	DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
	DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DestroyDevice);

	DXUTInit(true, true, nullptr); // Parse the command line, show msgboxes on error, no extra command line params
	DXUTSetCursorSettings(true, true); // Show the cursor and clip it when in full screen

	DXUTCreateWindow(L"Tutorial10");
	DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, m_ScreenWidth, m_ScreenHeight);
	DXUTMainLoop();

}

void DXUTHelper::shutdown()
{
	DXUTGetExitCode();
}

void DXUTHelper::render_text()
{
	m_pTextHelper->Begin();

	m_pTextHelper->SetInsertionPos(5, 5);
	m_pTextHelper->SetForegroundColor(Colors::Yellow);
	m_pTextHelper->DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
	m_pTextHelper->DrawTextLine(DXUTGetDeviceStats());

	m_pTextHelper->End();
}

void DXUTHelper::init_gui()
{
	m_fModelPuffiness = 0.0f;
	m_bSpinning = true;

	m_SettingsDlg.Init(&m_DialogResourceManager);
	m_HUD.Init(&m_DialogResourceManager);
	m_SampleUI.Init(&m_DialogResourceManager);

	m_HUD.SetCallback(OnGUIEvent); int iY = 10;
	m_HUD.AddButton(IDC_TOGGLEFULLSCREEN, L"Toggle full screen", 0, iY, 170, 22);
	m_HUD.AddButton(IDC_CHANGEDEVICE, L"Change device (F2)", 0, iY += 26, 170, 22, VK_F2);
	m_HUD.AddButton(IDC_TOGGLEREF, L"Toggle REF (F3)", 0, iY += 26, 170, 22, VK_F3);
	m_HUD.AddButton(IDC_TOGGLEWARP, L"Toggle WARP (F4)", 0, iY += 26, 170, 22, VK_F4);

	m_SampleUI.SetCallback(OnGUIEvent); iY = 10;

	WCHAR sz[100];
	iY += 24;
	swprintf_s(sz, 100, L"Puffiness: %0.2f", m_fModelPuffiness);
	m_SampleUI.AddStatic(IDC_PUFF_STATIC, sz, 0, iY += 26, 170, 22);
	m_SampleUI.AddSlider(IDC_PUFF_SCALE, 50, iY += 26, 100, 22, 0, 2000, ( int )( m_fModelPuffiness * 100.0f ));

	iY += 24;
	m_SampleUI.AddCheckBox(IDC_TOGGLESPIN, L"Toggle Spinning", 0, iY += 26, 170, 22, m_bSpinning);
}

void CALLBACK DXUTHelper::AppGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	switch ( nControlID )
	{
	case IDC_TOGGLEFULLSCREEN:
		DXUTToggleFullScreen();
		break;
	case IDC_TOGGLEREF:
		DXUTToggleREF();
		break;
	case IDC_TOGGLEWARP:
		DXUTToggleWARP();
		break;
	case IDC_CHANGEDEVICE:
		m_SettingsDlg.SetActive(!m_SettingsDlg.IsActive());
		break;

	case IDC_TOGGLESPIN:
	{
		m_bSpinning = m_SampleUI.GetCheckBox(IDC_TOGGLESPIN)->GetChecked();
		break;
	}

	case IDC_PUFF_SCALE:
	{
		WCHAR sz[100];
		m_fModelPuffiness = ( float )( m_SampleUI.GetSlider(IDC_PUFF_SCALE)->GetValue() * 0.01f );
		swprintf_s(sz, 100, L"Puffiness: %0.2f", m_fModelPuffiness);
		m_SampleUI.GetStatic(IDC_PUFF_STATIC)->SetText(sz);
		break;
	}

	}
}

//--------------------------------------------------------------------------------------
// Handle messages to the application
//--------------------------------------------------------------------------------------
LRESULT CALLBACK DXUTHelper::AppMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	bool* pbNoFurtherProcessing, void* pUserContext)
{
	// Pass messages to dialog resource manager calls so GUI state is updated correctly
	*pbNoFurtherProcessing = m_DialogResourceManager.MsgProc(hWnd, uMsg, wParam, lParam);
	if ( *pbNoFurtherProcessing )
		return 0;

	// Pass messages to settings dialog if its active
	if ( m_SettingsDlg.IsActive() )
	{
		m_SettingsDlg.MsgProc(hWnd, uMsg, wParam, lParam);
		return 0;
	}

	// Give the dialogs a chance to handle the message first
	*pbNoFurtherProcessing = m_HUD.MsgProc(hWnd, uMsg, wParam, lParam);
	if ( *pbNoFurtherProcessing )
		return 0;
	*pbNoFurtherProcessing = m_SampleUI.MsgProc(hWnd, uMsg, wParam, lParam);
	if ( *pbNoFurtherProcessing )
		return 0;

	// Pass all remaining windows messages to camera so it can respond to user input
	m_Camera.HandleMessages(hWnd, uMsg, wParam, lParam);
}


//--------------------------------------------------------------------------------------
// Handle key presses
//--------------------------------------------------------------------------------------
void CALLBACK DXUTHelper::AppKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	if (bKeyDown)
	{
		switch (nChar)
		{
		case VK_F1: // Change as needed                
			break;
		}
	}
}

//--------------------------------------------------------------------------------------
// Handle mouse button presses
//--------------------------------------------------------------------------------------
void CALLBACK DXUTHelper::AppMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
	bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
	int xPos, int yPos, void* pUserContext)
{
}


//--------------------------------------------------------------------------------------
// Call if device was removed.  Return true to find a new device, false to quit
//--------------------------------------------------------------------------------------
bool CALLBACK DXUTHelper::AppDeviceRemoved(void* pUserContext)
{
	return true;
}



//--------------------------------------------------------------------------------------
// Reject any D3D11 devices that aren't acceptable by returning false
//--------------------------------------------------------------------------------------
bool CALLBACK DXUTHelper::AppD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Called right before creating a D3D9 or D3D11 device, allowing the app to modify the device settings as needed
//--------------------------------------------------------------------------------------
bool CALLBACK DXUTHelper::AppModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return true;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that aren't dependant on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK DXUTHelper::AppD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	init_gui();

	HRESULT hr = S_OK;

	auto pD3D11DeviceContext = DXUTGetD3D11DeviceContext();
	V_RETURN( m_DialogResourceManager.OnD3D11CreateDevice(pd3dDevice, pD3D11DeviceContext) );
	V_RETURN( m_SettingsDlg.OnD3D11CreateDevice(pd3dDevice) );

	m_pTextHelper = new CDXUTTextHelper(pd3dDevice, pD3D11DeviceContext, &m_DialogResourceManager, 15);


	m_RenderSystem.init(pd3dDevice, pD3D11DeviceContext);

	// Setup the camera's view parameters
	static const XMVECTORF32 s_Eye ={0.0f, 3.0f, -800.0f, 0.f};
	static const XMVECTORF32 s_At ={0.0f, 1.0f, 0.0f, 0.f};
	m_Camera.SetViewParams(s_Eye, s_At);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Create any D3D11 resources that depend on the back buffer
//--------------------------------------------------------------------------------------
HRESULT CALLBACK DXUTHelper::AppD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	// Setup the projection parameters
	float fWidth = static_cast<float>(pBackBufferSurfaceDesc->Width);
	float fHeight = static_cast<float>(pBackBufferSurfaceDesc->Height);

	auto pD3D11DeviceContext = DXUTGetD3D11DeviceContext();

	m_RenderSystem.init_camera(fWidth, fHeight, pD3D11DeviceContext);

	HRESULT hr;

	V_RETURN(m_DialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(m_SettingsDlg.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc));

	// Setup the camera's projection parameters
	float fAspectRatio = fWidth / fHeight;
	m_Camera.SetProjParams(XM_PI/4, fAspectRatio, 0.1f, 1000.0f);
	m_Camera.SetWindow(pBackBufferSurfaceDesc->Width, pBackBufferSurfaceDesc->Height);
	m_Camera.SetButtonMasks(MOUSE_LEFT_BUTTON, MOUSE_WHEEL, MOUSE_MIDDLE_BUTTON);

	m_HUD.SetLocation(pBackBufferSurfaceDesc->Width-170, 0);
	m_HUD.SetSize(170, 170);
	m_SampleUI.SetLocation(pBackBufferSurfaceDesc->Width-170, pBackBufferSurfaceDesc->Height-300);
	m_SampleUI.SetSize(170, 300);

	return S_OK;
}

//--------------------------------------------------------------------------------------
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CALLBACK DXUTHelper::AppFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	// Rotate cube around the origin

	// Update the camera's position based on user input 
	m_Camera.FrameMove(fElapsedTime);

	XMMATRIX World;

	if ( m_bSpinning )
	{
		World = XMMatrixRotationY(60.0f * XMConvertToRadians(( float )fTime));
	}
	else
	{
		World = XMMatrixRotationY(XMConvertToRadians(180.f));
	}

	XMMATRIX mRot = XMMatrixRotationX(XMConvertToRadians(-90.0f));
	World = mRot * World;

	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(World) );

	m_RenderSystem.update();
}

//--------------------------------------------------------------------------------------
// Render the scene using the D3D11 device
//--------------------------------------------------------------------------------------
void CALLBACK DXUTHelper::AppD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
	double fTime, float fElapsedTime, void* pUserContext)
{
	// If the settings dialog is being shown, then render it instead of rendering the app's scene
	if ( m_SettingsDlg.IsActive() )
	{
		m_SettingsDlg.OnRender(fElapsedTime);
		return;
	}

	// Clear render target and the depth stencil 
	float ClearColor[4] ={ 0.2f, 0.3f, 1.0f, 0.0f };

	ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
	ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
	pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
	pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

	XMMATRIX View = m_Camera.GetViewMatrix();
	XMMATRIX Proj = m_Camera.GetProjMatrix();
	XMStoreFloat4x4(&m_Matrix.view, XMMatrixTranspose(View) );
	XMStoreFloat4x4(&m_Matrix.proj, XMMatrixTranspose(Proj) );
	
	m_RenderSystem.render(pd3dImmediateContext, m_Matrix);

	m_HUD.OnRender(fElapsedTime);
	m_SampleUI.OnRender(fElapsedTime);

	render_text();
}

//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11ResizedSwapChain 
//--------------------------------------------------------------------------------------
void CALLBACK DXUTHelper::AppD3D11ReleasingSwapChain(void* pUserContext)
{

}


//--------------------------------------------------------------------------------------
// Release D3D11 resources created in OnD3D11CreateDevice 
//--------------------------------------------------------------------------------------
void CALLBACK DXUTHelper::AppD3D11DestroyDevice(void* pUserContext)
{

	m_DialogResourceManager.OnD3D11DestroyDevice();
	m_SettingsDlg.OnD3D11DestroyDevice();
	DXUTGetGlobalResourceCache().OnDestroyDevice();

	SAFE_DELETE(m_pTextHelper);

	m_RenderSystem.shutdown();
}




bool CALLBACK DXUTHelper::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
{
	return pDXUTHelper->AppD3D11DeviceAcceptable(AdapterInfo, Output, DeviceInfo, BackBufferFormat, bWindowed, pUserContext);
}


bool CALLBACK DXUTHelper::ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
{
	return pDXUTHelper->AppModifyDeviceSettings(pDeviceSettings, pUserContext);
}



HRESULT CALLBACK DXUTHelper::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext)
{
	return pDXUTHelper->AppD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext);
}

HRESULT CALLBACK DXUTHelper::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
{
	return pDXUTHelper->AppD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc, pUserContext);
}



void CALLBACK DXUTHelper::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
	return pDXUTHelper->AppFrameMove(fTime, fElapsedTime, pUserContext);
}

void CALLBACK DXUTHelper::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
	double fTime, float fElapsedTime, void* pUserContext)
{
	return pDXUTHelper->AppD3D11FrameRender(pd3dDevice, pd3dImmediateContext, fTime, fElapsedTime, pUserContext);
}

void CALLBACK DXUTHelper::OnD3D11ReleasingSwapChain(void* pUserContext)
{
	return pDXUTHelper->AppD3D11ReleasingSwapChain(pUserContext);
}

void CALLBACK DXUTHelper::OnD3D11DestroyDevice(void* pUserContext)
{
	return pDXUTHelper->AppD3D11DestroyDevice(pUserContext);
}
void CALLBACK DXUTHelper::OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
	pDXUTHelper->AppGUIEvent(nEvent, nControlID, pControl, pUserContext);
}

LRESULT CALLBACK DXUTHelper::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
	bool* pbNoFurtherProcessing, void* pUserContext)
{
	return pDXUTHelper->AppMsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
}

void CALLBACK DXUTHelper::OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
{
	return pDXUTHelper->OnKeyboard(nChar, bKeyDown, bAltDown, pUserContext);
}

void CALLBACK DXUTHelper::OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
	bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
	int xPos, int yPos, void* pUserContext)
{
	return pDXUTHelper->AppMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down,
		bSideButton2Down, nMouseWheelDelta, xPos, yPos, pUserContext);
}

bool CALLBACK DXUTHelper::OnDeviceRemoved(void* pUserContext)
{
	return pDXUTHelper->AppDeviceRemoved(pUserContext);
}
}