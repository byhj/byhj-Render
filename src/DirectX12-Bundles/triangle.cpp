#include "triangle.h"

namespace byhj {

	void Triangle::init(ComPtr<ID3D12Device> pD3D12Device)
	{
		init_buffer(pD3D12Device);
		init_shader(pD3D12Device);
	}

	void Triangle::update()
	{
	}

	void Triangle::render(ComPtr<ID3D12GraphicsCommandList> pCommandList)
	{
		pCommandList->IASetVertexBuffers(VERTEX_BUFFER_SLOT, 1, &m_vertexBufferView);
		pCommandList->DrawInstanced(VERTEX_NUM, 1, 0, 0);
	}

	void Triangle::shutdown()
	{
	}
	ComPtr<ID3D12PipelineState> Triangle::getPipelineState()
	{
		return m_pPipelineState;
	}

	ComPtr<ID3D12RootSignature> Triangle::getRootSignature()
	{
		return m_pRootSignature;
	}

	/////////////////////////////////////////////////////////////
	void Triangle::init_buffer(ComPtr<ID3D12Device> pD3D12Device) 
	{
		//Create Vertex Buffer 

		Vertex VertexData[3] = {
			{ {  0.0f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
			{ {  0.5f, -0.5f, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
			{ { -0.5f, -0.5f, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
		};
		const UINT VertexBufferSize = sizeof(VertexData);

		//Note: using upload heaps to transfer static data like vertex buffers is not recommended.
		//Every time the GPU needs it, the upload heap will be marshalled over.
		//Please read up on Default Heap usage. An upload heap is used here for code simplicity and
		//because there are very few vertexs to atually transfer.
		HRESULT res;
		res = pD3D12Device->CreateCommittedResource(
			           &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 
			           D3D12_HEAP_FLAG_NONE,
			           &CD3DX12_RESOURCE_DESC::Buffer(VertexBufferSize),
			           D3D12_RESOURCE_STATE_GENERIC_READ,
			           nullptr,
			           IID_PPV_ARGS(&m_pVertexBuffer)
			           );
		assert(res == S_OK);

		//Copy the triangle data to the vertex buffer
		void *pVertexDataBegin = nullptr;
		res = m_pVertexBuffer->Map(0, nullptr, &pVertexDataBegin);
		memcpy(pVertexDataBegin, VertexData, VertexBufferSize);
		m_pVertexBuffer->Unmap(0, nullptr);

		//Initialize the vertex buffer view.
		m_vertexBufferView.BufferLocation = m_pVertexBuffer->GetGPUVirtualAddress();
		m_vertexBufferView.StrideInBytes  = sizeof(Vertex);
		m_vertexBufferView.SizeInBytes    = VertexBufferSize;
	}

	void Triangle::init_shader(ComPtr<ID3D12Device> pD3D12Device)
	{
		//Create a root signature
		CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
		ComPtr<ID3DBlob> signature;
		ComPtr<ID3DBlob> error;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error);
		pD3D12Device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_pRootSignature) );


		//Create the pipeline state, which includes compiling and loading shaders
		ComPtr<ID3DBlob> vertexShader;
		ComPtr<ID3DBlob> pixelShader;
#ifdef _DEBUG
		// Enable better shader debugging with the graphics debugging tools.
		UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		UINT compileFlags = 0;
#endif

		D3DCompileFromFile(L"triangle.vsh", nullptr, nullptr, "VS_MAIN", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
		D3DCompileFromFile(L"triangle.psh", nullptr, nullptr, "PS_MAIN", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);
		
		// Define the vertex input layout.
		D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
		};

		//Descrbe and create the graphics pipeline state object (PSO)
		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = {inputElementDescs, _countof(inputElementDescs) };
		psoDesc.pRootSignature = m_pRootSignature.Get();
		psoDesc.VS = {vertexShader->GetBufferPointer(), vertexShader->GetBufferSize() };
		psoDesc.PS = {pixelShader->GetBufferPointer(), pixelShader->GetBufferSize() };
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		HRESULT res = pD3D12Device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pPipelineState));
		assert(res == S_OK);
	}
}