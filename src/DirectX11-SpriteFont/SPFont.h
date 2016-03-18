#ifndef SPFont_H
#define SPFont_H

#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/SimpleMath.h>

#include <DirectXColors.h>
#include <DirectXMath.h>

using namespace DirectX;

#pragma  comment(lib, "DirectXTK")

namespace byhj {
  class SPFont {
  public:
	   SPFont() = default;
	  ~ SPFont() = default;
	  void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext,std::string fontName);
	  void render();


  private:
	  std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
      std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
  };
}

#endif