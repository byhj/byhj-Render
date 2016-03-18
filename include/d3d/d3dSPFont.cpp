#include "d3dSPFont.h"

namespace byhj {

	void  D3DSPFont::init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext, std::wstring fontName)
	{
		m_spriteFont = std::make_unique<DirectX::SpriteFont>(pD3D11Device, fontName);
		if (m_spriteFont == nullptr) {
		  std::cerr << "Error, fail to create the SpriteFont" << std::endl;
		}
	    m_spriteBatch = std::make_unique<DirectX::SpriteBatch>(pD3D11DeviceContext);
		if (m_spriteBatch == nullptr) {
			std::cerr << "Error, fail to create the SpriteBatch" << std::endl;
		}
	}

	void D3DSPFont::render(float posX, float posY, std::wstring msg)
	{
       DirectX::SimpleMath::Vector2 m_fontPos;
	   m_fontPos.x = posX;
	   m_fontPos.y = posY;

	   m_spriteBatch->Begin();

	   SimpleMath::Vector2 origin = m_spriteFont->MeasureString(msg.c_str());
	   m_spriteFont->DrawString(m_spriteBatch.get(), msg.c_str(), m_fontPos, DirectX::Colors::White, 0.0f, origin);

	   m_spriteBatch->End();
	}

	void D3DSPFont::shutdown()
	{

	}
}