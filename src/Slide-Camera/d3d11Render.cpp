#include "D3D11Render.h"
#include "windowInfo.h"

namespace byhj
{

	D3D11Render::D3D11Render()
	{

	}
	D3D11Render::~D3D11Render()
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
		m_camera.DetectInput(m_Timer.GetDeltaTime(), getHwnd());
		m_camera.UpdateSlideCamera(collidableGeometryPositions, collidableGeometryIndices);
	}

	void D3D11Render::v_render()
	{
		float bgColor[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };

		m_pD3D11DeviceContext->RSSetState(m_pRasterState);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
		
		XMMATRIX Scale = XMMatrixScaling(500.0f, 1.0f, 500.0f);
		XMMATRIX Translation = XMMatrixTranslation(0.0f, -5.0f, 0.0f);
		XMMATRIX groundWorld = Scale * Translation;
		XMStoreFloat4x4(&m_matrix.model, XMMatrixTranspose(groundWorld));

		m_matrix.view = m_camera.GetViewMatrix();
		m_plane.Render(m_pD3D11DeviceContext, m_matrix);

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
		bufferDesc.Width                   = WindowInfo::getInstance()->getWidth();
		bufferDesc.Height                  = WindowInfo::getInstance()->getHeight();
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

		depthStencilDesc.Width              = WindowInfo::getInstance()->getWidth();
		depthStencilDesc.Height             = WindowInfo::getInstance()->getHeight();
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
		vp.Width    = static_cast<FLOAT>(WindowInfo::getInstance()->getWidth() );
		vp.Height   = static_cast<FLOAT>(WindowInfo::getInstance()->getHeight() );
    	m_pD3D11DeviceContext->RSSetViewports(1, &vp);

		//MVP Matrix
		static auto aspect = WindowInfo::getInstance()->getAspect();
		XMVECTOR camPos    = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
		XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR camUp     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX View      = XMMatrixLookAtLH(camPos, camTarget, camUp);
		XMMATRIX Proj      = XMMatrixPerspectiveFovLH(0.4f*3.14f, aspect, 0.1f, 1000.0f);
		XMMATRIX Model     = XMMatrixIdentity();

		XMStoreFloat4x4(&m_matrix.model, XMMatrixTranspose(Model));
		XMStoreFloat4x4(&m_matrix.view, XMMatrixTranspose(View));
		XMStoreFloat4x4(&m_matrix.proj, XMMatrixTranspose(Proj));
    }
    
    void D3D11Render::init_object()
    {
    	m_Timer.Reset();
    	m_Font.init(m_pD3D11Device);
		m_plane.Init(m_pD3D11Device, m_pD3D11DeviceContext, getHwnd());
		m_camera.Init(getAppInst(), getHwnd());

		/************************************New Stuff****************************************************/
// Since our terrain will not be transformed throughout our scene, we will set the our groundWorlds
// world matrix here so that when we put the terrains positions in the "polygon soup", they will
// already be transformed to world space
		XMMATRIX Scale = XMMatrixScaling(500.0f, 1.0f, 500.0f);
		XMMATRIX Translation = XMMatrixTranslation(0.0f, -5.0f, 0.0f);
		XMMATRIX groundWorld = Scale * Translation;
		XMStoreFloat4x4(&m_matrix.model, XMMatrixTranspose(groundWorld));

		// Store the terrains vertex positions and indices in the
		// polygon soup that we will check for collisions with
		// We can store ALL static (non-changing) geometry in here that we want to check for collisions with
		int vertexOffset = collidableGeometryPositions.size();	// Vertex offset (each "mesh" will be added to the end of the positions array)

		// Temp arrays because we need to store the geometry in world space
		XMVECTOR tempVertexPosVec;
		XMFLOAT3 tempVertF3;
		auto v = m_plane.getVertexData();
		// Push back vertex positions to the polygon soup
		for (int i = 0; i < v.size(); i++)
		{
			tempVertexPosVec = XMLoadFloat3(&v[i]);
			tempVertexPosVec = XMVector3TransformCoord(tempVertexPosVec, groundWorld);
			XMStoreFloat3(&tempVertF3, tempVertexPosVec);
			collidableGeometryPositions.push_back(tempVertF3);
		}

		// Push back indices for polygon soup. We need to make sure we are
		// pushing back the indices "on top" of the previous pushed back
		// objects vertex positions, hence "+ vertexOffset" (This is the
		// first object we are putting in here, so it really doesn't
		// matter right now, but I just wanted to show you how to do it
		auto indices = m_plane.getIndexData();
		for (int i = 0; i < indices.size(); i++)
		{
			collidableGeometryIndices.push_back(indices[i] + vertexOffset);
		}
		/*************************************************************************************************/

    }
    
	void D3D11Render::drawfps()
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

		static WCHAR frameStr[255];
		wsprintfW(frameStr, L"FPS: %u", (UINT)fps);

		m_Font.render(m_pD3D11DeviceContext, frameStr, 20.0f, 10.0f, WindowInfo::getInstance()->getHeight() - 60);
	}
    void D3D11Render::drawInfo()
    {
    	WCHAR WinInfo[255];
    	swprintf(WinInfo, L"Window Size: %d x %d", WindowInfo::getInstance()->getWidth(), WindowInfo::getInstance()->getHeight() );
		drawfps();								  
     	m_Font.render(m_pD3D11DeviceContext, WinInfo, 20.0f, 10.0f, 30.0f);
     	m_Font.render(m_pD3D11DeviceContext, m_videoCardInfo.c_str(), 20.0f, 10.0f, 60.0f);
    }

}