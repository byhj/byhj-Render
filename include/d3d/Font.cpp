#include "Font.h"

namespace byhj
{

namespace d3d
{

void Font::drawFps(ID3D11DeviceContext *pD3D11DeviceContext, UINT fps)
{
	static WCHAR frameStr[255];
	wsprintfW(frameStr, L"FPS: %u", fps);

	m_pFontWrapper->DrawString(
		pD3D11DeviceContext,
		frameStr,// String
		22.0f,// Font size
		10.0f,// X position
		10.0f,// Y position
		0xff0099ff,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
}

void Font::drawText(ID3D11DeviceContext *pD3D11DeivceContext, const WCHAR *text, 
					   float fontSize, float posX, float posY)
{
	m_pFontWrapper->DrawString(
		pD3D11DeivceContext,
		text,// String
		fontSize,// Font size
		posX,// X position
		posY,// Y position
		0xff0099ff,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
		);
}

}

}