#pragma once

#include <d3d12.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <wrl.h>
#include <assert.h>
#include <allocators>
#include "d3d/d3dx12.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace byhj 
{
	class Triangle {
	public:
		void init(ComPtr<ID3D12Device> pD3D12Device);
		void update();
		void render(ComPtr<ID3D12GraphicsCommandList> pCommandList);
		void shutdown();

		ComPtr<ID3D12PipelineState> getPipelineState();
		ComPtr<ID3D12RootSignature> getRootSignature();

		struct Vertex {
			XMFLOAT3 pos;
			XMFLOAT4 color;
		};

	private:
		void init_buffer(ComPtr<ID3D12Device> pD3D12Device);
		void init_shader(ComPtr<ID3D12Device> pD3D12Device);

		struct OffsetBuffer
		{
			XMFLOAT4 offset;
		};
		OffsetBuffer m_offset;
		UINT8* m_pCbvDataBegin;

		ComPtr<ID3D12PipelineState> m_pPipelineState;
		ComPtr<ID3D12RootSignature> m_pRootSignature;
		ComPtr<ID3D12Resource>      m_pVertexBuffer;

		ComPtr<ID3D12DescriptorHeap> m_pCBVHeap;
		ComPtr<ID3D12Resource> m_pMatrixBuffer;

	    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		const UINT VERTEX_NUM = 3;
		const UINT VERTEX_BUFFER_SLOT = 0;

	};
}