#ifndef D3D12Render_H
#define D3D12Render_H

#include "d3d/d3dApp.h"
#include <d3d12.h>
#include <DirectXMath.h>
#include <dxgi1_4.h>
#include <windows.h>
#include <wrl.h>
#include "d3d/d3dx12.h"

#pragma comment(lib, "d3d12.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

namespace byhj
{
	class D3D12Render : public D3DApp
	{
	public:
		D3D12Render();
		~D3D12Render();

		void v_init();
		void v_update();
		void v_render();
		void v_shutdown();

	private:
		void populateCommandList();
		void waitForPreviousFrame();
		void loadAssets();
		void loadPipeline();

		static const UINT FrameCount = 2;

		ComPtr<IDXGISwapChain3>           m_pSwapChain;
		ComPtr<ID3D12Device>              m_pD3D12Device;
		ComPtr<ID3D12Fence>               m_pD3D12Fence;
		ComPtr<ID3D12Resource>            m_pRenderTargets[FrameCount];
		ComPtr<ID3D12CommandAllocator>    m_pCommandAllocator;
		ComPtr<ID3D12CommandQueue>        m_pCommandQueue;
		ComPtr<ID3D12DescriptorHeap>      m_pRTVHeap;
		ComPtr<ID3D12PipelineState>       m_pPipelineState;
		ComPtr<ID3D12GraphicsCommandList> m_pCommandList;

		UINT   m_RTVDescriptorSize;
		UINT   m_FrameIndex;
		HANDLE m_FenceEvent;
		UINT64 m_FenceValue;
	};
}
#endif