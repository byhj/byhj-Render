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
	    
		 m_model.render(pD3D11DeviceContext);
	    
		 m_meshShader.end();
	}

	void MeshLoad::shutdown()
	{
	}

	void MeshLoad::init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext)
	{
		m_model.loadModel("spaceCompound.obj");
		m_model.init(pD3D11Device, pD3D11DeviceContext);
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

		InputLayout.SemanticName         = "TEXCOORD";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32_FLOAT;
		InputLayout.InputSlot            = 0;
		InputLayout.AlignedByteOffset    = D3D11_APPEND_ALIGNED_ELEMENT;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);

		InputLayout.SemanticName         = "NORMAL";
		InputLayout.SemanticIndex        = 0;
		InputLayout.Format               = DXGI_FORMAT_R32G32_FLOAT;
		InputLayout.InputSlot            = 0;
		InputLayout.AlignedByteOffset    = 20;
		InputLayout.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		InputLayout.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(InputLayout);

		m_meshShader.init(pD3D11Device, vInputLayoutDesc);
		m_meshShader.attach(D3D_VERTEX_SHADER, L"model.vsh", "Model_VS", "vs_5_0");
		m_meshShader.attach(D3D_PIXEL_SHADER,  L"model.psh", "Model_PS", "ps_5_0");
		m_meshShader.end();
	}
}