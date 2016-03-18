#ifndef _D3DSPFont_H
#define _D3DSPFont_H

#include <DirectXTK/SpriteFont.h>
#include <DirectXTK/SimpleMath.h>

#include <DirectXColors.h>
#include <DirectXMath.h>
#include <iostream>

using namespace DirectX;

#pragma  comment(lib, "DirectXTK")

//Use Sprite Font with d3d11, your should create the sprite font file

namespace byhj {

  class D3DSPFont {
  public:
	   D3DSPFont() = default;
	  ~D3DSPFont() = default;

	  void init(ID3D11Device *pD3D11Device, ID3D11DeviceContext *pD3D11DeviceContext,std::wstring fontName = L"consolas.spritefont");
	  void render(float posX, float posY, std::wstring msg);
	  void shutdown();

  private:
	  std::unique_ptr<DirectX::SpriteFont> m_spriteFont;
      std::unique_ptr<DirectX::SpriteBatch> m_spriteBatch;
  };
}

#endif