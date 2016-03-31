#ifndef ShadowShader_H
#define ShadowShader_H

#include "d3d/d3dShader.h"

class ShadowShader
{
public:
	ShadowShader(){}
	~ShadowShader(){}

	void Init(ID3D11Device *pD3D11Device, HWND hWnd);
	void Use(ID3D11DeviceContext *pD3D11DeviceContext);

private:
	Shader shadowShader;
};
#endif
