#ifndef _D3FWFont_H
#define _D3FWFont_H

#include "font.h"
#include "FW1FontWrapper.h"

#pragma comment(lib, "FW1FontWrapper.lib")

//Use FW1FontWrapper in D3D11

namespace byhj {
	
    class D3DFWFont : public Font {
	public:
		 D3DFWFont();
		~D3DFWFont();

		void init(ID3D11Device *pD3D11Device, std::wstring fontFile = L"consolai");
		void render(ID3D11DeviceContext *pD3D11DeviceContext, std::wstring msg,
			         float fontSize = 20.0f, float posX = 10.0f, float posY = 10.0f);
		void shutdown();

	private:
		IFW1Factory *m_pFW1Factory;
		IFW1FontWrapper *m_pFontWrapper;
	};
}

#endif