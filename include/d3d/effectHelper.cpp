#include "effectHelper.h"

namespace byhj
{
	EffectHelper::EffectHelper()
	{
	}

	EffectHelper::~EffectHelper()
	{
	}

	void EffectHelper::init(std::string effectFile)
	{
		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#ifdef _DEBUG
		// Set the D3DCOMPILE_DEBUG flag to embed debug information in the shaders.
		// Setting this flag improves the shader debugging experience, but still allows 
		// the shaders to be optimized and to run exactly the way they will run in 
		// the release configuration of this program.
		dwShaderFlags |= D3DCOMPILE_DEBUG;

		// Disable optimizations to further improve shader debugging
		dwShaderFlags |= D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

#if D3D_COMPILER_VERSION >= 46
		// Read the D3DX effect file
		HRESULT hr = S_OK;
		D3DX11CompileEffectFromFile(L"tutorial11.fx", nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, dwShaderFlags, 0, pD3D11Device, &m_pEffect, nullptr);

#else
		ID3DBlob* pEffectBuffer = nullptr;
		V_RETURN(DXUTCompileFromFile(L"Tutorial11.fx", nullptr, "none", "fx_5_0", dwShaderFlags, 0, &pEffectBuffer));
		hr = D3DX11CreateEffectFromMemory(pEffectBuffer->GetBufferPointer(), pEffectBuffer->GetBufferSize(), 0, pd3dDevice, &m_pEffect);
		SAFE_RELEASE(pEffectBuffer);
		if (FAILED(hr))
			return hr;

#endif

		m_pEffectTechnique = m_pEffect->GetTechniqueByName("Render");
		m_pTexSRV          = m_pEffect->GetVariableByName("texDiffuse")->AsShaderResource();
		m_pModel           = m_pEffect->GetVariableByName("Model")->AsMatrix();
		m_pView            = m_pEffect->GetVariableByName("View")->AsMatrix();
		m_pProj            = m_pEffect->GetVariableByName("Proj")->AsMatrix();
		m_pWaviness        = m_pEffect->GetVariableByName("Waviness")->AsScalar();
		m_pTime            = m_pEffect->GetVariableByName("Time")->AsScalar();

		std::vector<D3D11_INPUT_ELEMENT_DESC> vInputLayoutDesc;
		D3D11_INPUT_ELEMENT_DESC inputLayoutDesc;

		inputLayoutDesc.SemanticName         = "POSITION";
		inputLayoutDesc.SemanticIndex        = 0;
		inputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesc.InputSlot            = 0;
		inputLayoutDesc.AlignedByteOffset    = 0;
		inputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(inputLayoutDesc);

		inputLayoutDesc.SemanticName         = "NORMAL";
		inputLayoutDesc.SemanticIndex        = 0;
		inputLayoutDesc.Format               = DXGI_FORMAT_R32G32B32_FLOAT;
		inputLayoutDesc.InputSlot            = 0;
		inputLayoutDesc.AlignedByteOffset    = 12;
		inputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(inputLayoutDesc);

		inputLayoutDesc.SemanticName         = "TEXCOORD";
		inputLayoutDesc.SemanticIndex        = 0;
		inputLayoutDesc.Format               = DXGI_FORMAT_R32G32_FLOAT;
		inputLayoutDesc.InputSlot            = 0;
		inputLayoutDesc.AlignedByteOffset    = 24;
		inputLayoutDesc.InputSlotClass       = D3D11_INPUT_PER_VERTEX_DATA;
		inputLayoutDesc.InstanceDataStepRate = 0;
		vInputLayoutDesc.push_back(inputLayoutDesc);

		// Create the input layout
		D3DX11_PASS_DESC PassDesc;
		auto numElements = vInputLayoutDesc.size();
		m_pEffectTechnique->GetPassByIndex(0)->GetDesc(&PassDesc);
		pD3D11Device->CreateInputLayout(&vInputLayoutDesc[0], numElements, PassDesc.pIAInputSignature,
			PassDesc.IAInputSignatureSize, &m_pInputLayout);
	}

	void EffectHelper::render()
	{

		// Set vertex buffer stride and offset.=
		unsigned int stride;
		unsigned int offset;
		stride = m_SdkMesh.GetVertexStride(0, 0);
		offset = 0;
		auto pVertexBuffer = m_SdkMesh.GetVB11(0, 0);
		auto pIndexBuffer = m_SdkMesh.GetIB11(0);
		auto IndexFormat = m_SdkMesh.GetIBFormat11(0);
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		pD3D11DeviceContext->IASetIndexBuffer(pIndexBuffer, IndexFormat, 0);

		pD3D11DeviceContext->IASetInputLayout(m_pInputLayout);

		m_pProj->SetMatrix((float*)&matrix.proj);
		m_pView->SetMatrix((float*)&matrix.view);
		m_pModel->SetMatrix((float*)&matrix.model);
		m_pTime->SetFloat((float)time);
		m_pWaviness->SetFloat((float)waveiness);

		D3DX11_TECHNIQUE_DESC techDesc;
		HRESULT hr;
		m_pEffectTechnique->GetDesc(&techDesc);

		for (UINT p = 0; p < techDesc.Passes; ++p)
		{

			for (UINT subIndex = 0; subIndex < m_SdkMesh.GetNumSubsets(0); ++subIndex)
			{
				auto pSubMesh = m_SdkMesh.GetSubset(0, subIndex);
				auto PrimType = m_SdkMesh.GetPrimitiveType11((SDKMESH_PRIMITIVE_TYPE)pSubMesh->PrimitiveType);
				pD3D11DeviceContext->IASetPrimitiveTopology(PrimType);

				//Ignores most of the material information in the mesh to use only a simple shader
				auto pTextureSRV = m_SdkMesh.GetMaterial(pSubMesh->MaterialID)->pDiffuseRV11;
				m_pTexSRV->SetResource(pTextureSRV);
				m_pEffectTechnique->GetPassByIndex(p)->Apply(0, pD3D11DeviceContext);

				auto IndexCount = (UINT)pSubMesh->IndexCount;
				auto VertexStart =  (UINT)pSubMesh->VertexStart;

				pD3D11DeviceContext->DrawIndexed(IndexCount, 0, VertexStart);
			}

		}
	}
}