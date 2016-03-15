#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include <windows.h>
#include <d3d11.h>
#include <DirectXColors.h>

#include "d3d/d3dUtility.h"
#include "Model.h"

using namespace DirectX;

namespace byhj
{

class RenderSystem 
{

public:
	void init(ID3D11Device *pD3D11Deivce, ID3D11DeviceContext *pD3D11DeviceContext);
	void update()   ;
	void render(ID3D11DeviceContext *pD3D11DeviceContext, D3DMVPMatrix m_Matrix);
	void shutdown() ;
	void init_camera(float width, float height, ID3D11DeviceContext *pD3D11DeviceContext);

private:
	void init_object(ID3D11Device *pD3D11Deivce, ID3D11DeviceContext *pD3D11DeviceContext);

private:

	Model m_Model;
	D3DMVPMatrix m_Matrix;
};


}

#endif