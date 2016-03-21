#include "MeshLoad.h"

namespace byhj {

	void MeshLoad::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd)
	{
		init_buffer(pD3D11Device, pD3D11DeviceContext);
		init_shader(pD3D11Device, hWnd);
	}

	void MeshLoad::update()
	{
	}

	void MeshLoad::render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix)
	{
		 m_meshShader.use(pD3D11DeviceContext);

		 cbMatrix.Model = matrix.model;
		 cbMatrix.View  = matrix.view;
		 cbMatrix.Proj  = matrix.proj;
		 pD3D11DeviceContext->UpdateSubresource(m_pMVPBuffer.Get(), 0, NULL, &cbMatrix, 0, 0);
		 pD3D11DeviceContext->VSSetConstantBuffers(0, 1, m_pMVPBuffer.GetAddressOf());

	    ModelMgr::getInstance()->render(pD3D11DeviceContext);

		 m_meshShader.end();
	}

	void MeshLoad::shutdown()
	{
	}

	void MeshLoad::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		ModelMgr::getInstance()->loadD3DModel("spaceCompound.obj", pD3D11Device, pD3D11DeviceContext);
	}


	void MeshLoad::init_shader(ID3D11Device *pD3D11Device, HWND hWnd)
	{
		HRESULT result;

		D3D11_INPUT_ELEMENT_DESC InputLayout;
		std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;

		InputLayout.SemanticName         = "POSITION";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		InputLayout.InputSlot            = 0;
		InputLayout.AlignedByteOffset    = 0;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);

		InputLayout.SemanticName         = "NORMAL";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		InputLayout.InputSlot            = 0;
		InputLayout.AlignedByteOffset    = 12;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);

		InputLayout.SemanticName         = "TEXCOORD";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32_FLOAT;
		InputLayout.InputSlot            = 24;
		InputLayout.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);

		m_meshShader.init(pD3D11Device, vInputLayoutDesc);
		m_meshShader.attach(D3D_VERTEX_SHADER, L"model.vsh", "Model_VS", "vs_5_0");
		m_meshShader.attach(D3D_PIXEL_SHADER,  L"model.psh", "Model_PS", "ps_5_0");
		m_meshShader.end();

		HRESULT hr;
		///////////////////////////////////////////////////////////////////
		D3D11_BUFFER_DESC mvpDesc;
		ZeroMemory(&mvpDesc, sizeof(D3D11_BUFFER_DESC));
		mvpDesc.Usage          = D3D11_USAGE_DEFAULT;
		mvpDesc.ByteWidth      = sizeof(byhj::MatrixBuffer);
		mvpDesc.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
		mvpDesc.CPUAccessFlags = 0;
		mvpDesc.MiscFlags      = 0;
		hr = pD3D11Device->CreateBuffer(&mvpDesc, NULL, &m_pMVPBuffer);
		
	}
}