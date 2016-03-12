#ifndef MeshLoad_H
#define MeshLoad_H

#include <d3d11.h>

#include "d3d/d3dShader.h"
#include "d3d/D3DUtility.h"
#include "d3dModel.h"

namespace byhj {

	class MeshLoad {
	public:
          void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, HWND hWnd);
		  void update();
		  void render(ID3D11DeviceContext *pD3D11DeviceContext, const D3DMVPMatrix &matrix);
		  void shutdown();

	private:
		void init_buffer(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext);
		void init_shader(ID3D11Device *pD3D11Device, HWND hWnd);

		D3DShader m_meshShader;
		D3DModel  m_model;

	};
}
#endif