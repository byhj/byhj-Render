#include "D3D12Render.h"
#include <WindowsX.h>

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

namespace byhj {

D3D12Render::D3D12Render() {

}

D3D12Render::~D3D12Render()
{
	if(m_pD3D12Device != nullptr) {
		flush_cmdQueue();
	}
}


bool D3D12Render::Get4xMsaaState() const
{
    return m_4xMsaaState;
}

void D3D12Render::Set4xMsaaState(bool value)
{
    if(m_4xMsaaState != value) {
        m_4xMsaaState = value;

        // Recreate the swapchain and buffers with new multisample settings.
		init_swapchain();
		init_size();
    }
}

void  D3D12Render::v_init()
{

	m_Timer.reset();
	m_Timer.start();

	init_d3d();
	init_size();

	// Reset the command list to prep for initialization commands.
	ThrowIfFailed(m_pCommandList->Reset(m_pCommandAlloc.Get(), nullptr));

	m_cube.init(m_pD3D12Device, m_pCommandList, m_pCommandQueue);

	// Execute the initialization commands.
	ThrowIfFailed(m_pCommandList->Close());
	ID3D12CommandList* cmdsLists[] ={ m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);
	flush_cmdQueue();

	

}

void  D3D12Render::v_update()
{
	m_Timer.count();
	m_cube.update(m_Timer.getTotalTime());

}

void  D3D12Render::v_render()
{
	// Reuse the memory associated with command recording.
	// We can only reset when the associated command lists have finished execution on the GPU.
	ThrowIfFailed(m_pCommandAlloc->Reset());

	// A command list can be reset after it has been added to the command queue via ExecuteCommandList.
	// Reusing the command list reuses memory.
	ThrowIfFailed(m_pCommandList->Reset(m_pCommandAlloc.Get(), m_cube.getPSO().Get()));

	m_pCommandList->RSSetViewports(1, &m_ScreenViewport);
	m_pCommandList->RSSetScissorRects(1, &m_ScissorRect);

	// Indicate a state transition on the resource usage.
	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Clear the back buffer and depth buffer.
	m_pCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Black, 0, nullptr);
	m_pCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	// Specify the buffers we are going to render to.
	m_pCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());


    m_cube.render();


	// Indicate a state transition on the resource usage.
	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// Done recording commands.
	ThrowIfFailed(m_pCommandList->Close());

	// Add the command list to the queue for execution.
	ID3D12CommandList* cmdsLists[] ={ m_pCommandList.Get() };
	m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// swap the back and front buffers
	ThrowIfFailed(m_pSwapChain->Present(0, 0));
	m_CurrBackBuffer = (m_CurrBackBuffer + 1) % m_SwapChainBufferCount;

	// Wait until frame commands are complete.  This waiting is inefficient and is
	// done for simplicity.  Later we will show how to organize our rendering code
	// so we do not have to wait per frame.
	flush_cmdQueue();
}

void  D3D12Render::v_shutdown()
{

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void D3D12Render::init_descHeaps()
{
	//create render target view heap
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
	rtvHeapDesc.NumDescriptors = m_SwapChainBufferCount;
	rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask       = 0;
    ThrowIfFailed(m_pD3D12Device->CreateDescriptorHeap( &rtvHeapDesc, IID_PPV_ARGS(m_pRendetTargetViewHeap.GetAddressOf())));


	//Create depth stencil heap
    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask       = 0;
    ThrowIfFailed(m_pD3D12Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(m_pDepthStencilViewHeap.GetAddressOf())));
}

void D3D12Render::init_size()
{
	assert(m_pD3D12Device);
	assert(m_pSwapChain);
    assert(m_pCommandAlloc);

	// Flush before changing any resources.
	flush_cmdQueue();

    ThrowIfFailed(m_pCommandList->Reset(m_pCommandAlloc.Get(), nullptr));

	// Release the previous resources we will be recreating.
	for (int i = 0; i < m_SwapChainBufferCount; ++i) {
		m_pSwapChainBuffer[i].Reset();
	}
    m_pDepthStencilBuffer.Reset();
	
	// Resize the swap chain.
    ThrowIfFailed(m_pSwapChain->ResizeBuffers(
		m_SwapChainBufferCount, 
		mClientWidth, mClientHeight, 
		m_BackBufferFormat, 
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	m_CurrBackBuffer = 0;
 
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(m_pRendetTargetViewHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < m_SwapChainBufferCount; i++) {
		ThrowIfFailed(m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&m_pSwapChainBuffer[i])));
		m_pD3D12Device->CreateRenderTargetView(m_pSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, m_RtvDescriptorSize);
	}    // Create the depth/stencil buffer and view.
    D3D12_RESOURCE_DESC depthStencilDesc;
    depthStencilDesc.Dimension          = D3D12_RESOURCE_DIMENSION_TEXTURE2D;


    depthStencilDesc.Alignment          = 0;
    depthStencilDesc.Width              = mClientWidth;
    depthStencilDesc.Height             = mClientHeight;
    depthStencilDesc.DepthOrArraySize   = 1;
    depthStencilDesc.MipLevels          = 1;
    depthStencilDesc.Format             = m_DepthStencilFormat;
    depthStencilDesc.SampleDesc.Count   = m_4xMsaaState ? 4 : 1;
    depthStencilDesc.SampleDesc.Quality = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
    depthStencilDesc.Layout             = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    depthStencilDesc.Flags              = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optClear;
    optClear.Format = m_DepthStencilFormat;
    optClear.DepthStencil.Depth = 1.0f;
    optClear.DepthStencil.Stencil = 0;
    ThrowIfFailed(m_pD3D12Device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
        &depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
        &optClear,
        IID_PPV_ARGS(m_pDepthStencilBuffer.GetAddressOf())));

    // Create descriptor to mip level 0 of entire resource using the format of the resource.
    m_pD3D12Device->CreateDepthStencilView(m_pDepthStencilBuffer.Get(), nullptr, DepthStencilView());

    // Transition the resource from its initial state to be used as a depth buffer.
	m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pDepthStencilBuffer.Get(),
		                            D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	
    // Execute the resize commands.
    ThrowIfFailed(m_pCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { m_pCommandList.Get() };
    m_pCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until resize is complete.
	flush_cmdQueue();

	// Update the viewport transform to cover the client area.
	m_ScreenViewport.TopLeftX = 0;
	m_ScreenViewport.TopLeftY = 0;
	m_ScreenViewport.Width    = static_cast<float>(mClientWidth);
	m_ScreenViewport.Height   = static_cast<float>(mClientHeight);
	m_ScreenViewport.MinDepth = 0.0f;
	m_ScreenViewport.MaxDepth = 1.0f;

    m_ScissorRect = { 0, 0, mClientWidth, mClientHeight };
}


void D3D12Render::init_d3d()
{
#if defined(DEBUG) || defined(_DEBUG) 
	// Enable the D3D12 debug layer.
{
	ComPtr<ID3D12Debug> debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
}
#endif

	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&m_pFactory)));

	// Try to create hardware device.
	HRESULT hardwareResult = D3D12CreateDevice(
		nullptr,             // default adapter
		D3D_FEATURE_LEVEL_11_0,
		IID_PPV_ARGS(&m_pD3D12Device));

	// Fallback to WARP device.
	if(FAILED(hardwareResult))
	{
		ComPtr<IDXGIAdapter> pWarpAdapter;
		ThrowIfFailed(m_pFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			pWarpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_pD3D12Device)));
	}

	ThrowIfFailed(m_pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE,
		IID_PPV_ARGS(&m_pFence)));

	m_RtvDescriptorSize       = m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_DsvDescriptorSize       = m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	m_CbvSrvUavDescriptorSize = m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // Check 4X MSAA quality support for our back buffer format.
    // All Direct3D 11 capable devices support 4X MSAA for all render 
    // target formats, so we only need to check quality support.

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format           = m_BackBufferFormat;
	msQualityLevels.SampleCount      = 4;
	msQualityLevels.Flags            = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	ThrowIfFailed(m_pD3D12Device->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels)));

    m_4xMsaaQuality = msQualityLevels.NumQualityLevels;
	assert(m_4xMsaaQuality > 0 && "Unexpected MSAA quality level.");
	
#ifdef _DEBUG
    LogAdapters();
#endif

	init_cmd();
	init_swapchain();
	init_descHeaps();

}

void D3D12Render::init_cmd()
{
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	ThrowIfFailed(m_pD3D12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue)));

	ThrowIfFailed(m_pD3D12Device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(m_pCommandAlloc.GetAddressOf())));

	ThrowIfFailed(m_pD3D12Device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_pCommandAlloc.Get(), // Associated command allocator
		nullptr,                   // Initial PipelineStateObject
		IID_PPV_ARGS(m_pCommandList.GetAddressOf())));

	// Start off in a closed state.  This is because the first time we refer 
	// to the command list we will Reset it, and it needs to be closed before
	// calling Reset.
	m_pCommandList->Close();
}

void D3D12Render::init_swapchain()
{
    // Release the previous swapchain we will be recreating.
    m_pSwapChain.Reset();

    DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width                   = mClientWidth;
	sd.BufferDesc.Height                  = mClientHeight;
	sd.BufferDesc.RefreshRate.Numerator   = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format                  = m_BackBufferFormat;
	sd.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count                   = m_4xMsaaState ? 4 : 1;
	sd.SampleDesc.Quality                 = m_4xMsaaState ? (m_4xMsaaQuality - 1) : 0;
	sd.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount                        = m_SwapChainBufferCount;
	sd.OutputWindow                       = getHwnd();
	sd.Windowed                           = true;
	sd.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// Note: Swap chain uses queue to perform flush.
    ThrowIfFailed(m_pFactory->CreateSwapChain(
		m_pCommandQueue.Get(),
		&sd, 
		m_pSwapChain.GetAddressOf()));
}

void D3D12Render::flush_cmdQueue()
{
	// Advance the fence value to mark commands up to this fence point.
    m_CurrentFence++;

    // Add an instruction to the command queue to set a new fence point.  Because we 
	// are on the GPU timeline, the new fence point won't be set until the GPU finishes
	// processing all the commands prior to this Signal().
    ThrowIfFailed(m_pCommandQueue->Signal(m_pFence.Get(), m_CurrentFence));

	// Wait until the GPU has completed commands up to this fence point.
    if(m_pFence->GetCompletedValue() < m_CurrentFence)
	{
		HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);

        // Fire event when GPU hits current fence.  
        ThrowIfFailed(m_pFence->SetEventOnCompletion(m_CurrentFence, eventHandle));

        // Wait until the GPU hits current fence event is fired.
		WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
	}
}

ID3D12Resource* D3D12Render::CurrentBackBuffer()const
{
	return m_pSwapChainBuffer[m_CurrBackBuffer].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12Render::CurrentBackBufferView()const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_pRendetTargetViewHeap->GetCPUDescriptorHandleForHeapStart(),
		m_CurrBackBuffer,
		m_RtvDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE D3D12Render::DepthStencilView()const
{
	return m_pDepthStencilViewHeap->GetCPUDescriptorHandleForHeapStart();
}

void D3D12Render::calc_fps()
{
	// Code computes the average frames per second, and also the 
	// average time it takes to render one frame.  These stats 
	// are appended to the window caption bar.
    
	static int frameCnt = 0;
	static float timeElapsed = 0.0f;

	frameCnt++;

	// Compute averages over one second period.
	if( (m_Timer.getTotalTime() - timeElapsed) >= 1.0f )
	{
		float fps = (float)frameCnt; // fps = frameCnt / 1
		float mspf = 1000.0f / fps;

        wstring fpsStr = to_wstring(fps);
        wstring mspfStr = to_wstring(mspf);

        wstring windowText =
            L"    fps: " + fpsStr +
            L"   mspf: " + mspfStr;

        SetWindowText(getHwnd(), windowText.c_str());
		
		// Reset for next average.
		frameCnt = 0;
		timeElapsed += 1.0f;
	}
}

void D3D12Render::LogAdapters()
{
    UINT i = 0;
    IDXGIAdapter* adapter = nullptr;
    std::vector<IDXGIAdapter*> adapterList;
    while(m_pFactory->EnumAdapters(i, &adapter) != DXGI_ERROR_NOT_FOUND) {
        DXGI_ADAPTER_DESC desc;
        adapter->GetDesc(&desc);

        std::wstring text = L"***Adapter: ";
        text += desc.Description;
        text += L"\n";

        OutputDebugString(text.c_str());

        adapterList.push_back(adapter);
        
        ++i;
    }

    for(size_t i = 0; i < adapterList.size(); ++i) {
        LogAdapterOutputs(adapterList[i]);
        ReleaseCom(adapterList[i]);
    }
}

void D3D12Render::LogAdapterOutputs(IDXGIAdapter* adapter)
{
    UINT i = 0;
    IDXGIOutput* output = nullptr;
    while(adapter->EnumOutputs(i, &output) != DXGI_ERROR_NOT_FOUND)  {
        DXGI_OUTPUT_DESC desc;
        output->GetDesc(&desc);
        
        std::wstring text = L"***Output: ";
        text += desc.DeviceName;
        text += L"\n";
        OutputDebugString(text.c_str());

        LogOutputDisplayModes(output, m_BackBufferFormat);

        ReleaseCom(output);

        ++i;
    }
}

void D3D12Render::LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format)
{
    UINT count = 0;
    UINT flags = 0;

    // Call with nullptr to get list count.
    output->GetDisplayModeList(format, flags, &count, nullptr);

    std::vector<DXGI_MODE_DESC> modeList(count);
    output->GetDisplayModeList(format, flags, &count, &modeList[0]);

    for(auto& x : modeList) {
        UINT n = x.RefreshRate.Numerator;
        UINT d = x.RefreshRate.Denominator;
        std::wstring text =
            L"Width = " + std::to_wstring(x.Width) + L" " +
            L"Height = " + std::to_wstring(x.Height) + L" " +
            L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
            L"\n";

        ::OutputDebugString(text.c_str());
    }
}

}