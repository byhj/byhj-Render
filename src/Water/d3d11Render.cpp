#include "D3D11Render.h"
#include "windowInfo.h"

namespace byhj
{

	D3D11Render::D3D11render()
	{

	}
	D3D11Render::~D3D11render()
	{

	}

	void D3D11Render::v_init()
	{
		init_device();
		init_camera();
		init_object();


	}

	void D3D11Render::v_update()
	{
		D3DEulerCamera::getInstance()->detectInput(m_Timer.getDeltaTime(), getHwnd());
	}

	void D3D11Render::v_render()
	{
		float bgColor[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };

		m_pD3D11DeviceContext->RSSetState(m_pRasterState);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//m_matrix.view = D3DSphereCamera::getInstance()->getViewMat();
		m_matrix.view = D3DEulerCamera::getInstance()->getViewMat();
		BeginScene();
		static float rot = 0.0f;
		rot +=  m_Timer.GetDeltaTime();
		UpdateScene();
		m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
		m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
		//////////////////////Render Refraction To Texture/////////////////////////////
		float blackColor[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
		// Setup a clipping plane based on the height of the water to clip everything above it.
		XMFLOAT4 clipPlane = XMFLOAT4(0.0f, -1.0f, 0.0f, 2.75f + 0.1f);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pRefractRTV.GetAddressOf(), m_pDepthStencilView.Get());
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blackColor);

		XMMATRIX World = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(World));
		m_RefracShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
		m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"marble01.dds").GetAddressOf());
		bathModel.render(m_pD3D11DeviceContext.Get());


		//////////////////////Render Reflection To Texture////////////////////////////////
		m_pD3D11DeviceContext->RSSetState(m_pRasterState.Get());
		m_pD3D11DeviceContext->OMSetDepthStencilState(m_pDepthStencilState.Get(), 1);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, m_pReflectRTV.GetAddressOf(), m_pDepthStencilView.Get());
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView.Get(), blackColor);

		// Translate to where the wall model will be rendered.
		World = XMMatrixTranslation(0.0f, 6.0f, 8.0f);
		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(World));

		XMFLOAT4X4 View  = m_Camera.GetViewMatrix();

		XMFLOAT3 camPos = m_Camera.GetPos();
		XMFLOAT3 camTarget = m_Camera.GetTarget();
		XMFLOAT3 camUp = m_Camera.GetUp();
		camPos.y =  -camPos.y;
		XMVECTOR pos    =  XMLoadFloat3(&camPos);
		XMVECTOR target =  XMLoadFloat3(&camTarget);
		XMVECTOR up     =  XMLoadFloat3(&camUp);
		XMMATRIX reflectMat = XMMatrixLookAtLH(pos, target, up);
		XMFLOAT4X4 Reflect;
		XMStoreFloat4x4(&Reflect, XMMatrixTranspose(reflectMat));
		m_Matrix.view = Reflect;

		m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
		m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"wall01.dds").GetAddressOf());
		wallModel.render(m_pD3D11DeviceContext.Get());

		///////////////////////////////Render Scene///////////////////////////////////////

		BeginScene();

		m_Matrix.view = m_Camera.GetViewMatrix();

		/////////////////////Render the scene Model//////////////////////////////
		XMMATRIX Model = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
		m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
		m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"ground01.dds").GetAddressOf());
		groundModel.render(m_pD3D11DeviceContext.Get());

		Model = XMMatrixTranslation(0.0f, 6.0f, 8.0f);
		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
		m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
		m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"wall01.dds").GetAddressOf());
		wallModel.render(m_pD3D11DeviceContext.Get());

		Model = XMMatrixTranslation(0.0f, 2.0f, 0.0f);
		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
		m_LightShader.use(m_pD3D11DeviceContext.Get(), m_Matrix);
		m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"marble01.dds").GetAddressOf());
		bathModel.render(m_pD3D11DeviceContext.Get());

		/////////////////////////////////////////////////////////////////////////////


		Model = XMMatrixTranslation(0.0f, 2.75f, 0.0f);
		XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
		m_WaterShader.use(m_pD3D11DeviceContext.Get(), m_Matrix, Reflect);
		m_pD3D11DeviceContext->PSSetShaderResources(0, 1, m_TextureMgr.GetTexture(L"water01.dds").GetAddressOf());
		m_pD3D11DeviceContext->PSSetShaderResources(1, 1, m_pReflectSRV.GetAddressOf());
		m_pD3D11DeviceContext->PSSetShaderResources(2, 1, m_pRefractSRV.GetAddressOf());
		waterModel.render(m_pD3D11DeviceContext.Get());


		drawInfo();

		m_pSwapChain->Present(0, 0);
	}

	void D3D11Render::v_shutdown()
	{
	}


	void D3D11Render::init_device()
	{

		////////////////////////Create buffer desc////////////////////////////
		DXGI_MODE_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
		bufferDesc.Width                   = getClientWidth();
		bufferDesc.Height                  = getClientHeight();
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
		swapChainDesc.OutputWindow       = getHwnd();
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


		///////////////////////////Creaete Depth/Stencil Buffer/////////////////////////////////
		D3D11_TEXTURE2D_DESC depthStencilDesc;

		depthStencilDesc.Width              = getClientWidth();
		depthStencilDesc.Height             = getClientHeight();
		depthStencilDesc.MipLevels          = 1;
		depthStencilDesc.ArraySize          = 1;
		depthStencilDesc.Format             = DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthStencilDesc.SampleDesc.Count   = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
		depthStencilDesc.Usage              = D3D11_USAGE_DEFAULT;
		depthStencilDesc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
		depthStencilDesc.CPUAccessFlags     = 0;
		depthStencilDesc.MiscFlags          = 0;

		m_pD3D11Device->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer);
		m_pD3D11Device->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);


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
		m_videoCardInfo = std::wstring(L"Video Card: ") + adapterDesc.Description;

	}

	void D3D11Render::init_camera()
	{
		//view port Information
		D3D11_VIEWPORT vp;
		ZeroMemory(&vp, sizeof(D3D11_VIEWPORT));
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.Width    = static_cast<FLOAT>(getClientWidth());
		vp.Height   = static_cast<FLOAT>(getClientHeight());
		m_pD3D11DeviceContext->RSSetViewports(1, &vp);

		//MVP Matrix
		static auto aspect = WindowInfo::getInstance()->getAspect();
		XMVECTOR camPos    = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR camUp     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX View      = XMMatrixLookAtLH(camPos, camTarget, camUp);
		XMMATRIX Proj      = XMMatrixPerspectiveFovLH(0.4f*3.14f, aspect, 1.0f, 1000.0f);
		XMMATRIX Model     = XMMatrixRotationY(60.0f);

		XMStoreFloat4x4(&m_matrix.model, XMMatrixTranspose(Model));
		XMStoreFloat4x4(&m_matrix.view, XMMatrixTranspose(View));
		XMStoreFloat4x4(&m_matrix.proj, XMMatrixTranspose(Proj));

	}

	void D3D11Render::init_object()
	{
		m_Timer.reset();
		m_Font.init(m_pD3D11Device);
		bathModel.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());
		groundModel.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());
		wallModel.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());
		waterModel.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get());


		m_LightShader.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd());

		m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"marble01.dds");
		m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"ground01.dds");
		m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"wall01.dds");
		m_TextureMgr.LoadTexture(m_pD3D11Device.Get(), L"water01.dds");
		m_WaterShader.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd());
		m_RefracShader.init(m_pD3D11Device.Get(), m_pD3D11DeviceContext.Get(), GetHwnd());

		m_Timer.Reset();
		m_Font.init(m_pD3D11Device.Get());
		m_Camera.SetRadius(15.0f);
	}

	void D3D11Render::drawfps()
	{
		static bool flag = true;
		if (flag)
		{
			m_Timer.start();
			flag = false;
		}

		m_Timer.count();
		static int frameCnt = 0;
		static float timeElapsed = 0.0f;
		frameCnt++;
		if (m_Timer.getTotalTime() - timeElapsed >= 1.0f)
		{
			fps = frameCnt;
			frameCnt = 0;
			timeElapsed += 1.0f;
		}

		static WCHAR frameStr[255];
		wsprintfW(frameStr, L"FPS: %u", (UINT)fps);

		m_Font.render(m_pD3D11DeviceContext, frameStr, 22.0f, 10.0f, WindowInfo::getInstance()->getHeight() - 60);
	}
	void D3D11Render::drawInfo()
	{
		WCHAR WinInfo[255];
		swprintf(WinInfo, L"Window Size: %d x %d", WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight());
		drawfps();
		m_Font.render(m_pD3D11DeviceContext, WinInfo, 22.0f, 10.0f, 20.0f);
		m_Font.render(m_pD3D11DeviceContext, m_videoCardInfo.c_str(), 22.0f, 10.0f, 60.0f);
	}

}