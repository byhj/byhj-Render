#include "d3d12Render.h"

namespace byhj {

	void D3D12Render::v_init()
	{
		m_viewport.Width = static_cast<float>(m_ScreenWidth);
		m_viewport.Height = static_cast<float>(m_ScreenHeight);
		m_viewport.MaxDepth = 1.0f;

		m_scissorRect.right = static_cast<LONG>(m_ScreenWidth);
		m_scissorRect.bottom = static_cast<LONG>(m_ScreenHeight);

		loadPipeline();
		loadAssets();

		m_triangle.init(m_pD3D12Device);
	}

	void D3D12Render::v_update()
	{
	}

	void D3D12Render::v_render()
	{
		//Record all the commands we need to render the scene into the command list
		populateCommandList();

		//Execute the comman d list
		ID3D12CommandList *ppCommandLists[] = { m_pCommandList.Get() };
		m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		HRESULT res = m_pSwapChain->Present(1, 0);
		assert(res == S_OK);

		waitForPreviousFrame();
	}

	void D3D12Render::v_shutdown()
	{
	    waitForPreviousFrame();
		m_triangle.shutdown();
	    //CloseHandle(m_fenceEvent);
	}

	void D3D12Render::loadPipeline()
	{
		HRESULT res;
#ifdef _DEBUG
		//Enable the D3D12 debug layer
		ComPtr<ID3D12Debug> pDebugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&pDebugController))))
		{
			pDebugController->EnableDebugLayer();
		}
#endif

		ComPtr<IDXGIFactory4> pFactory;
		CreateDXGIFactory1(IID_PPV_ARGS(&pFactory));

		bool UseWarpDevice = false;

		if (UseWarpDevice)
		{
			ComPtr<IDXGIAdapter> pWarpAdapter;
			res = pFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter));
			assert(res == S_OK);
			res = D3D12CreateDevice(pWarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pD3D12Device));
			assert(res == S_OK);
		}
		else
		{
			res = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_pD3D12Device));
		    assert(res == S_OK);
		}

		///////////////////////	//Describe and Create the command queue//////////////////////////////////////////////

		D3D12_COMMAND_QUEUE_DESC queueDesc ={};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;
		res =  m_pD3D12Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_pCommandQueue));
		assert(res == S_OK);

		//Describe and Create the swap chain
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
		res =  pFactory->CreateSwapChain(
			m_pCommandQueue.Get(),		// Swap chain needs the queue so that it can force a flush on it.
			&swapChainDesc,
			&pSwapChain
			);
		assert(res == S_OK);

		res = pSwapChain.As(&m_pSwapChain);
		assert(res == S_OK);

		m_frameIndex = m_pSwapChain->GetCurrentBackBufferIndex();

		///////////////////////// Create Decriptor heaps /////////////////////////////////////////////


		{
			D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc ={};
			rtvHeapDesc.NumDescriptors = FrameCount;
			rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
			rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

			res =  m_pD3D12Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_pRTVHeap));
			assert(res == S_OK);
			m_rtvDescSize = m_pD3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		}

		//Create frame resources
		{
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart());

			// Create a RTV for each frame.
			for (UINT n = 0; n < FrameCount; n++)
			{
				res =  m_pSwapChain->GetBuffer(n, IID_PPV_ARGS(&m_pRenderTargets[n]));
				assert(res == S_OK);
				m_pD3D12Device->CreateRenderTargetView(m_pRenderTargets[n].Get(), nullptr, rtvHandle);
				rtvHandle.Offset(1, m_rtvDescSize);
			}
		}
		//The command allocator is going to be used for allocating memory for the list of commands that we send to the GPU each frame to render graphics.
		res = m_pD3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_pCommandAllocator));
		assert(res == S_OK);
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
			// We use the fence as a signaling mechanism to notify us when the GPU is completely done rendering the command list that we submitted via the command queue
			m_pD3D12Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_pD3D12Fence));
			m_fenceValue = 1;

			// Create an event handle to use for frame synchronization.
			m_fenceEvent = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
			if (m_fenceEvent == nullptr)
			{
				HRESULT_FROM_WIN32(GetLastError());
			}

			// Wait for the command list to execute; we are reusing the same command 
			// list in our main loop but for now, we just want to wait for setup to 
			// complete before continuing.
			waitForPreviousFrame();
		}
	}


	void D3D12Render::populateCommandList()
	{
		//Command list allocators can only be reset when the associated 
		//command lists have finished execution on the gpu, apps should use
		//fences to determine gpu execution progress.
		m_pCommandAllocator->Reset();

		//However, when executeCommandList() is called on a particular command list
		//that command list can then be reset at any time and must be before re-recoring
		auto pPipelineState = m_triangle.getPipelineState();
		m_pCommandList->Reset(m_pCommandAllocator.Get(), pPipelineState.Get());
		auto pRootSignature = m_triangle.getRootSignature();
		m_pCommandList->SetGraphicsRootSignature(pRootSignature.Get());

		m_pCommandList->RSSetViewports(1, &m_viewport);
		m_pCommandList->RSSetScissorRects(1, &m_scissorRect);
		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_pRTVHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescSize);
		m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);


		// Record commands.
		const float clearColor[] ={ 0.0f, 0.0f, 0.0f, 1.0f };
		m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		m_pCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_triangle.render(m_pCommandList);

		//pCIndicate that the back buffer will now be used to present.
		m_pCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_pRenderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

		m_pCommandList->Close();
	}
	void D3D12Render::waitForPreviousFrame()
	{
		//Waiting for the frame to complete before continuing is not best parctice.
		//This is code implemented as such for simplicity, more advanced samples illustrate how 
		//to use fences for efficient resource usage.

		//Signal and increment the fence value
		const UINT64 fence = m_fenceValue;
		m_pCommandQueue->Signal(m_pD3D12Fence.Get(), fence);
		++m_fenceValue;

		//Wait until the previout frame is finished
		if (m_pD3D12Fence->GetCompletedValue() < fence) {
			m_pD3D12Fence->SetEventOnCompletion(fence, m_fenceEvent);
			WaitForSingleObject(m_fenceEvent, INFINITE);
		}
		m_frameIndex = m_pSwapChain->GetCurrentBackBufferIndex();
	}
}