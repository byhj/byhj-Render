#include "RenderSystem.h"

namespace byhj
{

RenderSystem::RenderSystem() 
{
	m_AppName = L"DirectX11-Init D3D";
}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::v_Init()
{
	init_device();
	init_camera();
	init_object();
}

void RenderSystem::v_Update()
{
}

void RenderSystem::v_Render()
{



	BeginScene();

	UpdateScene();
	m_Matrix.view = m_Camera.GetViewMatrix();

	m_Plane.Render(m_pD3D11DeviceContext.Get(), m_Matrix);

	DrawInfo();

	EndScene();

}

void RenderSystem::v_Shutdown()
{

	m_Plane.Shutdown();
}


void RenderSystem::UpdateScene()
{
	m_Camera.update();
}
void  RenderSystem::v_OnMouseDown(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseDown(btnState, x, y, GetHwnd());
}

void  RenderSystem::v_OnMouseMove(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseMove(btnState, x, y);
}

void  RenderSystem::v_OnMouseUp(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseUp(btnState, x, y);
}
void  RenderSystem::v_OnMouseWheel(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseWheel(btnState, x, y, GetAspect());
}
void RenderSystem::init_device()
{

	////////////////////////Create buffer desc////////////////////////////
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width                   = m_ScreenWidth;
	bufferDesc.Height                  = m_ScreenHeight;
	bufferDesc.RefreshRate.Numerator   = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

	//Create swapChain Desc
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc         = bufferDesc;
	swapChainDesc.SampleDesc.Count   = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount        = 1;
	swapChainDesc.OutputWindow       = GetHwnd();
	swapChainDesc.Windowed           = TRUE;
	swapChainDesc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;


	///////////////////////////////////////////////////////////////////////////
	HRESULT hr;
	//Create the double buffer chain
	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE,
		NULL, NULL, NULL, NULL, D3D11_SDK_VERSION,
		&swapChainDesc, &m_pSwapChain, &m_pD3D11Device,
		NULL, &m_pD3D11DeviceContext);
	//DebugHR(hr);

	//Create back buffer, buffer also is a texture
	ID3D11Texture2D *pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	hr = m_pD3D11Device->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();
	//DebugHR(hr);
	
	//////////////////////////// Initialize the description of the stencil state.///////////////////////////////////////////////
	D3D11_TEXTURE2D_DESC depthBufferDesc;
	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width              = m_ScreenWidth;
	depthBufferDesc.Height             = m_ScreenHeight;
	depthBufferDesc.MipLevels          = 1;
	depthBufferDesc.ArraySize          = 1;
	depthBufferDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count   = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage              = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags     = 0;
	depthBufferDesc.MiscFlags          = 0;
	// Create the texture for the depth buffer using the filled out description.
	hr = m_pD3D11Device->CreateTexture2D(&depthBufferDesc, NULL, &m_pDepthStencilBuffer);


	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable      = true;
	depthStencilDesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc        = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable    = true;
	depthStencilDesc.StencilReadMask  = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	hr = m_pD3D11Device->CreateDepthStencilState(&depthStencilDesc, &m_pDepthStencilState);
	// Set the depth stencil state.

	//DebugHR(hr);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	// Initialize the depth stencil view.
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	// Set up the depth stencil view description.
	depthStencilViewDesc.Format =  DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create the depth stencil view.
	hr = m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), &depthStencilViewDesc, &m_pDepthStencilView);
	//DebugHR(hr);

	// ////////////Clear the second depth stencil state before setting the parameters.//////////////////////
	D3D11_DEPTH_STENCIL_DESC depthDisabledStencilDesc;
	ZeroMemory(&depthDisabledStencilDesc, sizeof(depthDisabledStencilDesc));
	// Now create a second depth stencil state which turns off the Z buffer for 2D rendering.  The only difference is 
	// that DepthEnable is set to false, all other parameters are the same as the other depth stencil state.
	depthDisabledStencilDesc.DepthEnable = false;
	depthDisabledStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthDisabledStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthDisabledStencilDesc.StencilEnable = true;
	depthDisabledStencilDesc.StencilReadMask = 0xFF;
	depthDisabledStencilDesc.StencilWriteMask = 0xFF;
	depthDisabledStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthDisabledStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthDisabledStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthDisabledStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthDisabledStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the state using the device.
	hr = m_pD3D11Device->CreateDepthStencilState(&depthDisabledStencilDesc, &m_pDepthDisabledStencilState);
	//DebugHR(hr);

	// Setup the raster description which will determine how and what polygons will be drawn.
	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode              = D3D11_CULL_BACK;
	rasterDesc.DepthBias             = 0;
	rasterDesc.DepthBiasClamp        = 0.0f;
	rasterDesc.DepthClipEnable       = true;
	rasterDesc.FillMode              = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable     = false;
	rasterDesc.ScissorEnable         = false;
	rasterDesc.SlopeScaledDepthBias  = 0.0f;
	// Create the rasterizer state from the description we just filled out.
	hr = m_pD3D11Device->CreateRasterizerState(&rasterDesc, &m_pRasterState);
	//DebugHR(hr);

	///////////////////////////////////////////////////////////////////////////////

	unsigned int numModes, i, numerator, denominator, stringLength;
	IDXGIFactory* factory;
	IDXGIAdapter* adapter;
	IDXGISurface *surface;
	DXGI_ADAPTER_DESC adapterDesc;

	// Create a DirectX graphics interface factory.
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	// Use the factory to create an adapter for the primary graphics interface (video card).
	factory->EnumAdapters(0, &adapter);
	adapter->GetDesc(&adapterDesc);
	m_videoCardMemory = (int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	// Convert the name of the video card to a character array and store it.
	m_videoCardInfo = std::wstring(L"Video Card  :") + adapterDesc.Description;

}

void RenderSystem::BeginScene()
{
	//Render 
	float bgColor[4] ={ 0.2f, 0.3f, 0.4f, 1.0f };

	m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
	m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());
	m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), bgColor);
	m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

}

void RenderSystem::EndScene()
{
	m_pSwapChain->Present(0, 0);
}

void RenderSystem::init_camera()
{
	//Viewport Infomation
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width    = static_cast<FLOAT>(m_ScreenWidth);
	vp.Height   = static_cast<FLOAT>(m_ScreenHeight);
	m_pD3D11DeviceContext->RSSetViewports(1, &vp);

	//MVP Matrix
	XMVECTOR camPos    = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR camUp     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX View      = XMMatrixLookAtLH(camPos, camTarget, camUp);
	XMMATRIX Proj      = XMMatrixPerspectiveFovLH(0.4f*3.14f, GetAspect(), 1.0f, 1000.0f);
	XMMATRIX Model     = XMMatrixRotationY(60.0f);


	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model) );
	XMStoreFloat4x4(&m_Matrix.view,  XMMatrixTranspose(View) );
	XMStoreFloat4x4(&m_Matrix.proj,  XMMatrixTranspose(Proj) );

}

void RenderSystem::init_object()
{

	m_Timer.Reset();
	m_Plane.Init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd());
	m_Font.init(m_pD3D11Device.Get());
	m_Camera.SetRadius(5.0f);
}


void RenderSystem::TurnZBufferOn()
{
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
	return;
}


void RenderSystem::TurnZBufferOff()
{
	m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthDisabledStencilState.Get(), 1);
	return;
}
void RenderSystem::DrawFps()
{
	static bool flag = true;
	if (flag)
	{
		m_Timer.Start();
		flag = false;
	}

	m_Timer.Count();
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;
	frameCnt++;
	if (m_Timer.GetTotalTime() - timeElapsed >= 1.0f)
	{
		fps = frameCnt;
		frameCnt = 0;
		timeElapsed += 1.0f;
	}

	m_Font.drawFps(m_pD3D11DeviceContext.Get(), (UINT)fps);
}

void RenderSystem::DrawInfo()
{
	WCHAR WinInfo[255];
	swprintf(WinInfo, L"Window Size: %d x %d", m_ScreenWidth, m_ScreenHeight);
	DrawFps();
	m_Font.drawText(m_pD3D11DeviceContext.Get(), WinInfo, 22.0f, 10.0f, 40.0f);
	m_Font.drawText(m_pD3D11DeviceContext.Get(), m_videoCardInfo.c_str(), 22.0f, 10.0f, 70.0f);
}

}