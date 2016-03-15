#ifndef CUBE_H
#define CUBE_H

#include <d3d11.h>

#include <DirectXMath.h>
using namespace DirectX;

#include "d3dx11effect.h"
#include "d3d/d3dUtility.h"
#include "EffectHelper.h"
#include "Light.h"

namespace byhj
{


	class Cube
	{
	public:
		Cube();
		~Cube();

		void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		void render(ID3D11DeviceContext *pD3D11DeviceContext, D3DMVPMatrix matrix);
		void shutdown();

	private:
		void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);
		void init_light();
		void init_texture(ID3D11Device *pD3D11Device);

		D3DMVPMatrix cbMatrix;
		struct  Vertex
		{
			XMFLOAT3 Pos;
			XMFLOAT3 Normal;
			XMFLOAT2 Tex;
		};
		EffectHelper m_EffectHelper;
		struct LightBuffer
		{
			d3d::DirectionLight g_DirLights[2];
			XMFLOAT4       g_EyePos;
		};
		LightBuffer cbLight;
		d3d::DirectionLight m_DirLights[2];
		d3d::Material m_BoxMat;

		ID3D11Buffer                *m_pCubeVB        = nullptr;
		ID3D11Buffer                *m_pCubeIB        = nullptr;
		ID3D11ShaderResourceView    *m_pDiffuseTexSRV = nullptr;

		int m_VertexCount = 0;
		int m_IndexCount  = 0;
	};

}


#endif