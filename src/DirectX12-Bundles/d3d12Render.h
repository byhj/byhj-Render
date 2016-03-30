#pragma once

#include "d3d/d3dApp.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi1_4.h>
#include <windows.h>
#include <wrl.h>

#include "d3d/d3dx12.h"
#include "triangle.h"

#pragma comment(lib, "d3d12")

using namespace DirectX;
using namespace Microsoft::WRL;

namespace byhj {

	class D3D12Render : public D3DApp {
	public:
		void v_init()     override;
		void v_update()   override;
		void v_render()   override;
		void v_shutdown() override;

	private:
		void populateCommandList();
		void waitForPreviousFrame();
		void loadAssets();
		void loadPipeline();

		static const UINT FrameCount = 2;
		D3D12_VIEWPORT m_viewport;
		D3D12_RECT     m_scissorRect;

		ComPtr<IDXGISwapChain3>           m_pSwapChain;
		ComPtr<ID3D12Device>              m_pD3D12Device;
		ComPtr<ID3D12Fence>               m_pD3D12Fence;
		ComPtr<ID3D12Resource>            m_pRenderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator>    m_pCommandAllocator;
		ComPtr<ID3D12CommandAllocator>    m_pBundleAllocator;
		ComPtr<ID3D12CommandQueue>        m_pCommandQueue;
		ComPtr<ID3D12DescriptorHeap>      m_pRTVHeap;
		ComPtr<ID3D12GraphicsCommandList> m_pCommandList;
		ComPtr<ID3D12GraphicsCommandList> m_pBundleList;

		UINT   m_rtvDescSize;
		UINT   m_frameIndex;
		HANDLE m_fenceEvent;
		UINT64 m_fenceValue;

		Triangle m_triangle;
	};
}
