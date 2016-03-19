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

		// create renderer and enable extra states
		Direct3D11Renderer& cegui_renderer = Direct3D11Renderer::create(m_pD3D11Device,m_pD3D11DeviceContext);
	//	cegui_renderer.enableExtraStateSettings(true);

		// create CEGUI system object
		CEGUI::System::create(cegui_renderer);

		// setup resource directories
		DefaultResourceProvider* rp = static_cast<DefaultResourceProvider*>(System::getSingleton().getResourceProvider());
		rp->setResourceGroupDirectory("schemes", "../../media/datafiles/schemes/");
		rp->setResourceGroupDirectory("imagesets", "../../media/datafiles/imagesets/");
		rp->setResourceGroupDirectory("fonts", "../../media/datafiles/fonts/");
		rp->setResourceGroupDirectory("layouts", "../../media/datafiles/layouts/");
		rp->setResourceGroupDirectory("looknfeels", "../../media/datafiles/looknfeel/");
		rp->setResourceGroupDirectory("lua_scripts", "../../media/datafiles/lua_scripts/");
		rp->setResourceGroupDirectory("schemas", "../../media/datafiles/xml_schemas/");

		// set default resource groups
		ImageManager::setImagesetDefaultResourceGroup("imagesets");
		CEGUI::Font::setDefaultResourceGroup("fonts");
		Scheme::setDefaultResourceGroup("schemes");
		WidgetLookManager::setDefaultResourceGroup("looknfeels");
		WindowManager::setDefaultResourceGroup("layouts");
		ScriptModule::setDefaultResourceGroup("lua_scripts");

		XMLParser* parser = System::getSingleton().getXMLParser();
		if (parser->isPropertyPresent("SchemaDefaultResourceGroup"))
			parser->setProperty("SchemaDefaultResourceGroup", "schemas");

		// load TaharezLook scheme and DejaVuSans-10 font
		SchemeManager::getSingleton().createFromFile("TaharezLook.scheme", "schemes");
		FontManager::getSingleton().createFromFile("DejaVuSans-10.font");

		// set default font and cursor image and tooltip type
		System::getSingleton().getDefaultGUIContext().setDefaultFont("DejaVuSans-10");
		System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
		System::getSingleton().getDefaultGUIContext().setDefaultTooltipType("TaharezLook/Tooltip");
	
		/////////////////////////////////////////////////////////////
	// Add your gui initialisation code in here.
	// You should preferably use layout loading because you won't
	// have to recompile everytime you change the layout. But you
	// can also use static window creation code here, of course.
	/////////////////////////////////////////////////////////////

		System::getSingleton().notifyDisplaySizeChanged(Sizef(800.f, 600.f));

	// load layout
		Window* root = WindowManager::getSingleton().loadLayoutFromFile("application_templates.layout");
		System::getSingleton().getDefaultGUIContext().setRootWindow(root);
		// notify system of the window size
		renderer= static_cast<Direct3D11Renderer*>(System::getSingleton().getRenderer());
	}

	void D3D11Render::v_update()
	{

	}

	void D3D11Render::v_render()
	{
		float bgColor[4] ={ 0.0f, 0.0f, 0.0f, 1.0f };

		m_pD3D11DeviceContext->RSSetState(m_pRasterState);
		m_pD3D11DeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);
		m_pD3D11DeviceContext->ClearRenderTargetView(m_pRenderTargetView, bgColor);
		m_pD3D11DeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



		// render gui
		renderer->beginRendering();
		System::getSingleton().renderAllGUIContexts();
		renderer->endRendering();



		//drawInfo();

		m_pSwapChain->Present(0, 0);
	}

    void D3D11Render::v_shutdown()
    {
		// destroy system and renderer
		System::destroy();
		Direct3D11Renderer::destroy(*renderer);
		renderer = 0;
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
    
    }
    
    void D3D11Render::init_object()
    {
    	m_Timer.reset();
    	m_Font.init(m_pD3D11Device);
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