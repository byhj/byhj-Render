#ifndef EFFECTHELPER_H
#define EFFECTHELPER_H

#include <d3dx11effect.h>
#include "d3d/d3dUtility.h"

#include <DirectXMath.h> 
using namespace DirectX;

#pragma comment(lib, "Effects11.lib")

namespace byhj
{

	class EffectHelper
	{
	public:
		EffectHelper();
		~EffectHelper();

	public:

		void init(ID3D11Device *pD3D11Device);
		void render(ID3D11DeviceContext *pD3D11DeviceContext);
		void shutdown();

		void setWorld(const XMFLOAT4X4 &world);
		void setView(const XMFLOAT4X4 &view);
		void setProj(const XMFLOAT4X4 &proj);
		void setDiffuseTex(ID3D11ShaderResourceView *pDiffuseSRV);
		void setEyePos(const XMFLOAT4 &eyePos);

		ID3DX11Effect          *m_pEffect            = nullptr;
		ID3DX11EffectTechnique *m_pEffectLight1Tech  = nullptr;
		ID3DX11EffectTechnique *m_pEffectLight2Tech  = nullptr;
		ID3DX11EffectTechnique *m_pEffectLight3Tech  = nullptr;

		ID3DX11EffectVariable  *m_pFxDirLight    = nullptr;
		ID3DX11EffectVariable  *m_pFxMaterial    = nullptr;
		ID3DX11EffectMatrixVariable *m_pFxWorld  = nullptr;
		ID3DX11EffectMatrixVariable *m_pFxView   = nullptr;
		ID3DX11EffectMatrixVariable *m_pFxProj   = nullptr;
		ID3DX11EffectVectorVariable *m_pFxEyePos = nullptr;
		ID3DX11EffectShaderResourceVariable * m_pFxDiffuseMap    = nullptr;

		ID3D11InputLayout           *m_pInputLayout = nullptr;
	};

}

#endif
