#ifndef D3DFont_H
#define D3DFont_H

#include "font.h"
#include "FW1FontWrapper.h"

#pragma comment(lib, "FW1FontWrapper.lib")

namespace byhj {
	
    class D3DFont : public Font {
	public:
		D3DFont();
		~D3DFont();

		void init(ID3D11Device *pD3D11Device);
		void render(ID3D11DeviceContext *pD3D11DeviceContext, const WCHAR *pText,
			         float fontSize = 5.0f, float posX = 10.0f, float posY = 10.0f);
	private:
		IFW1Factory *m_pFW1Factory;
		IFW1FontWrapper *m_pFontWrapper;
	};
}

#endif