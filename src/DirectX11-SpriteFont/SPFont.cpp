#include "SPFont.h"

namespace byhj {

	void  SPFont::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, std::string fontName)
	{
		m_spriteFont = std::make_unique<DirectX::SpriteFont>(pD3D11Device, L"consolas.spritefont");
	    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pD3D11DeviceContext);
	}

	void SPFont::render()
	{
       DirectX::SimpleMath::Vector2 m_fontPos;
	   m_fontPos.x = 200.0f;
	   m_fontPos.y = 50.0f;

	   m_spriteBatch->Begin();

	   const wchar_t *output = L"Hello World";
	   SimpleMath::Vector2 origin = m_spriteFont->MeasureString(output);
	   m_spriteFont->DrawString(m_spriteBatch.get(), output, m_fontPos, DirectX::Colors::White, 0.0f, origin);

	   m_spriteBatch->End();
	}
}