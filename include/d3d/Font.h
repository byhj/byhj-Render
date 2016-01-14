#ifndef Font_H
#define Font_H

#include <FW1FontWrapper.h>
#pragma  comment(lib, "FW1FontWrapper.lib")

#include "d3dDebug.h"

namespace byhj
{

namespace d3d
{


class Font
{
public:
	Font(): m_pFW1Factory(NULL), m_pFontWrapper(NULL) {}

    void Init(ID3D11Device *pD3D11Device)
	{
		HRESULT hr = FW1CreateFactory(FW1_VERSION, &m_pFW1Factory);
		hr = m_pFW1Factory->CreateFontWrapper(pD3D11Device, L"Arial", &m_pFontWrapper);
		DebugHR(hr);
		m_pFW1Factory->Release();
	}
	void drawText(ID3D11DeviceContext *pD3D11DeivceContext, const WCHAR *text, 
		          float fontSize, float posX, float posY);

	void drawFps(ID3D11DeviceContext *pD3D11DeviceContext, UINT fps);

private:
	IFW1Factory     *m_pFW1Factory;
	IFW1FontWrapper *m_pFontWrapper;
};

}

}

#endif
