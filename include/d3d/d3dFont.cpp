#include "d3dFont.h"

namespace byhj
{
	D3DFont::D3DFont()
	{
	}

	D3DFont::~D3DFont()
	{
	}

	void D3DFont::init(ID3D11Device *pD3D11Device)
	{
		HRESULT hr = FW1CreateFactory(FW1_VERSION, &m_pFW1Factory);
		hr = m_pFW1Factory->CreateFontWrapper(pD3D11Device, L"consolai", &m_pFontWrapper);
	}

	void D3DFont::render(ID3D11DeviceContext *pD3D11DeviceContext, const WCHAR *pText,
		                 float fontSize /* = 10.0f */, float posX /* = 10.0f */, float posY /* = 10.0f */)
	{
		m_pFontWrapper->DrawString(
			pD3D11DeviceContext,
			pText,// String
			fontSize,// Font size
			posX,// X position
			posY,// Y position
			0xff0099ff,// Text color, 0xAaBbGgRr
			FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
			);
	}
}