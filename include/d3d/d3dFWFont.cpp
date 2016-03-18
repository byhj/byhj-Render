#include "d3dFWFont.h"

namespace byhj
{
	D3DFWFont::D3DFWFont()
	{
	}

	D3DFWFont::~D3DFWFont()
	{
	}

	void D3DFWFont::init(ID3D11Device *pD3D11Device, std::wstring fontFile)
	{
		HRESULT hr = FW1CreateFactory(FW1_VERSION, &m_pFW1Factory);
		hr = m_pFW1Factory->CreateFontWrapper(pD3D11Device, fontFile.c_str(), &m_pFontWrapper);

		m_pFW1Factory->Release();
	}

	void D3DFWFont::render(ID3D11DeviceContext *pD3D11DeviceContext, std::wstring msg,
		                 float fontSize /* = 22.0f */, float posX /* = 10.0f */, float posY /* = 10.0f */)
	{
		m_pFontWrapper->DrawString(
			pD3D11DeviceContext,
			msg.c_str(),// String
			fontSize,// Font size
			posX,// X position
			posY,// Y position
			//0xff0099ff,// Text color, 0xAaBbGgRr
			0xffffffff,
			FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
			);
	}
	void D3DFWFont::shutdown() 
	{

	}
}