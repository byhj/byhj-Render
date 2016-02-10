#include "D3D12Render.h"
#include "windowInfo.h"

namespace byhj
{

	D3D12Render::D3D12Render()
	{

	}
	D3D12Render::~D3D12Render()
	{

	}
	void D3D12Render::v_init()
	{
		loadPipeline();
		loadAssets();
	}

	void D3D12Render::v_update()
	{

	}

	void D3D12Render::v_render()
	{
		populateCommandList();

		//Execute the command list
		ID3D12CommandList *ppCommandLists[] ={ m_pCommandList.Get() };
		m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		//Present the frame
		m_pSwapChain->Present(1, 0);

		waitForPreviousFrame();
	}

	void D3D12Render::v_shutdown()
	{
		waitForPreviousFrame();
	}


	void D3D12Render::loadPipeline()
	{
#ifdef _DEBUG
		ComPtr<ID3D12Debug> pDebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController)))) {
			pDebugController->EnableDebugLayer();
		}
#endif
		ComPtr<IDXGIFactory4> pFactory;
		CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));
		bool userWrapDevice = false;
		if (userWrapDevice) {
			ComPtr<IDXGIAdapter> pWrapAdapter;
			pFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWrapAdapter));
			D3D12CreateDevice(pWrapAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pD3D12Device));
		} else {
			D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pD3D12Device));
		}

		////////////////////////Describe and Create the command queue//////////////////////////////////////////////
		D3D12_COMMAND_QUEUE_DESC queueDesc ={};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
		m_pD3D12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));

		////////////////////////////Describe and Create the swap chain///////////////////////////////////////

		DXGI_SWAP_CHAIN_DESC swapChainDesc ={};
		swapChainDesc.BufferCount       = FrameCount;
		swapChainDesc.BufferDesc.Width  = m_ScreenWidth;
		swapChainDesc.BufferDesc.Height = m_ScreenHeight;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage       = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect        = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.OutputWindow      = getHwnd();
		swapChainDesc.SampleDesc.Count  = 1;
		swapChainDesc.Windowed          = TRUE;

		ComPtr<IDXGISwapChain> pSwapChain;
		pFactory->CreateSwapChain(m_pCommandQueue.Get(), &swapChainDesc, &pSwapChain);
		pSwapChain.As(&m_pSwapChain);
		m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		///////////////////////// Create Descriptor heaps /////////////////////////////////////////////
		{
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc ={};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
			m_pD3D12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRTVHeap));
			m_RTVDescriptorSize = m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		}

		///////////////////////////////////////Create frame resources///////////////////////////////////////
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV for each frame.
			for (UINT n = 0; n < FrameCount; n++)
			{
				m_pSwapChain->GetBuffer(n, IID_PPV_ARGS(&m_pRenderTargets[n]));
				m_pD3D12Device->CreateRenderTargetView(m_pRenderTargets[n].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, m_RTVDescriptorSize);
			}
		}
		m_pD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));

	
	}
	void D3D12Render::loadAssets()
	{
		// Create the command list.
		m_pD3D12Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_pCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_pCommandList));

		// Command lists are created in the recording state, but there is nothing
		// to record yet. The main loop expects it to be closed, so close it now.
		m_pCommandList->Close();

		// Create synchronization objects.
		{
			m_pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pD3D12Fence));
			m_FenceValue = 1;

			// Create an event handle to use for frame synchronization.
			m_FenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
			if (m_FenceEvent == nullptr)
			{
				HRESULT_FROM_WIN32(GetLastError());
			}
		}
	}
	void D3D12Render::populateCommandList()
	{
		// Command list allocators can only be reset when the associated 
		// command lists have finished execution on the GPU; apps should use 
		// fences to determine GPU execution progress.
		m_pCommandAllocator->Reset();

		// However, when ExecuteCommandList() is called on a particular command 
		// list, that command list can then be reset at any time and must be before 
		// re-recording.
		m_pCommandList->Reset(m_pCommandAllocator.Get(), m_pPipelineState.Get());


		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart(), m_FrameIndex, m_RTVDescriptorSize);
		m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		// Record commands.
		const float clearColor[] ={ 0.0f, 0.2f, 0.4f, 1.0f };
		m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		//pCIndicate that the back buffer will now be used to present.
		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargets[m_FrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		m_pCommandList->Close();


	}

	void D3D12Render::waitForPreviousFrame()
	{
		// WAITING FOR THE FRAME TO COMPLETE BEFORE CONTINUING IS NOT BEST PRACTICE.
		// This is code implemented as such for simplicity. More advanced samples 
		// illustrate how to use fences for efficient resource usage.
		// Signal and increment the fence value.
		const UINT64 fence = m_FenceValue;
		m_pCommandQueue->Signal(m_pD3D12Fence.Get(), fence);
		m_FenceValue++;

		// Wait until the previous frame is finished.
		if (m_pD3D12Fence->GetCompletedValue() < fence)
		{
			m_pD3D12Fence->SetEventOnCompletion(fence, m_FenceEvent);
			WaitForSingleObject(m_FenceEvent, INFINITE);
		}

		m_FrameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

	}
}