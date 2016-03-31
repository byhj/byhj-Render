
#include "TextureMgr.h"

namespace byhj
{

	void TextureMgr::LoadTexture(ID3D11Device *pD3D11Device, std::wstring texFile)
	{
		static int i = 0;
      HRESULT hr;
	  ComPtr<ID3D11ShaderResourceView> pTexture;
	  hr = CreateDDSTextureFromFile(pD3D11Device, (m_TextureDir + texFile).c_str(), NULL, &pTexture);
	  m_Textures.push_back(pTexture);
	  m_TextureMapIndex.insert(std::make_pair(texFile, i++));
	}

	ComPtr<ID3D11ShaderResourceView> TextureMgr::GetTexture(std::wstring texName)
	{
		auto index = m_TextureMapIndex[texName];
		return m_Textures[index];
	}
}