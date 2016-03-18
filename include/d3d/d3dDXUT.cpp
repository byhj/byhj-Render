#include "dxutHelper.h"

namespace byhj
{

	DXUTHelper::DXUTHelper()
	{

	}

	DXUTHelper::~DXUTHelper()
	{

	}

	std::shared_ptr<DXUTHelper> getInsance() 
	{
		static std::shared_ptr<DXUTHelper> pInstance = std::make_shared<DXUTHelper>();
        
		if (pInstance != nullptr) {
		   return pInstance; 
		}
	}

	void DXUTHelper::init()
	{
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
		DXUTCreateWindow(L"DXUTHelper");
		DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, m_ScreenWidth, m_ScreenHeight);
		DXUTMainLoop();

	}

	void DXUTHelper::shutdown()
	{
		DXUTGetExitCode();
	}


	bool CALLBACK DXUTHelper::IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
		DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
	{
		return DXUTHelper::getInsance()->AppD3D11DeviceAcceptable(AdapterInfo, Output, DeviceInfo, BackBufferFormat, bWindowed, pUserContext);
	}

	bool CALLBACK DXUTHelper::ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
	{
		return DXUTHelper::getInsance()->AppModifyDeviceSettings(pDeviceSettings, pUserContext);
	}

	HRESULT CALLBACK DXUTHelper::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc, pUserContext);
	}

	HRESULT CALLBACK DXUTHelper::OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc, pUserContext);
	}


	void CALLBACK DXUTHelper::OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppFrameMove(fTime, fElapsedTime, pUserContext);
	}

	void CALLBACK DXUTHelper::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
		double fTime, float fElapsedTime, void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppD3D11FrameRender(pd3dDevice, pd3dImmediateContext, fTime, fElapsedTime, pUserContext);
	}

	void CALLBACK DXUTHelper::OnD3D11ReleasingSwapChain(void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppD3D11ReleasingSwapChain(pUserContext);
	}

	void CALLBACK DXUTHelper::OnD3D11DestroyDevice(void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppD3D11DestroyDevice(pUserContext);
	}

	LRESULT CALLBACK DXUTHelper::MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppMsgProc(hWnd, uMsg, wParam, lParam, pbNoFurtherProcessing, pUserContext);
	}

	void CALLBACK DXUTHelper::OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
	{
		return  DXUTHelper::getInsance()->OnKeyboard(nChar, bKeyDown, bAltDown, pUserContext);
	}

	void CALLBACK DXUTHelper::OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
		bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
		int xPos, int yPos, void* pUserContext)
	{
		return  DXUTHelper::getInsance()->AppMouse(bLeftButtonDown, bRightButtonDown, bMiddleButtonDown, bSideButton1Down,
			bSideButton2Down, nMouseWheelDelta, xPos, yPos, pUserContext);
	}

	bool CALLBACK DXUTHelper::OnDeviceRemoved(void* pUserContext)
	{
		return DXUTHelper::getInsance()->AppDeviceRemoved(pUserContext);
	}

	//--------------------------------------------------------------------------------------
	// Handle messages to the application
	//--------------------------------------------------------------------------------------
	LRESULT CALLBACK DXUTHelper::AppMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext)
	{
		return 0;
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

		auto pD3D11DeviceContext = DXUTGetD3D11DeviceContext();
		//g_RenderSystem.Init(pd3dDevice, pD3D11DeviceContext);

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

		//g_RenderSystem.init_camera(fWidth, fHeight, pD3D11DeviceContext);


		return S_OK;
	}

	//--------------------------------------------------------------------------------------
	// Handle updates to the scene.  This is called regardless of which D3D API is used
	//--------------------------------------------------------------------------------------
	void CALLBACK DXUTHelper::AppFrameMove(double fTime, float fElapsedTime, void* pUserContext)
	{
		//g_RenderSystem.Update();
	}

	//--------------------------------------------------------------------------------------
	// Render the scene using the D3D11 device
	//--------------------------------------------------------------------------------------
	void CALLBACK DXUTHelper::AppD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
		double fTime, float fElapsedTime, void* pUserContext)
	{
		// Clear render target and the depth stencil 
		float ClearColor[4] ={ 0.2f, 0.3f, 1.0f, 0.0f };

		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		pd3dImmediateContext->ClearRenderTargetView(pRTV, ClearColor);
		pd3dImmediateContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

		//g_RenderSystem.Render(pd3dImmediateContext);
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
		DXUTGetGlobalResourceCache().OnDestroyDevice();

		//g_RenderSystem.Shutdown();
	}

}