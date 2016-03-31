#ifndef Textur_H
#define Textur_H

#include <unordered_map>
#include <d3d11.h>
#include <string>
#include <vector>
#include <wrl.h>
#include "d3d/Utility.h"

using namespace Microsoft::WRL;


namespace byhj
{
	class TextureMgr
	{
	public:
		TextureMgr() = default;
		~TextureMgr() = default;


		void LoadTexture(ID3D11Device *pD3D11Device,  std::wstring texFile);
		ComPtr<ID3D11ShaderResourceView> GetTexture(std::wstring texName);
		
	private:
		std::wstring m_TextureDir = L"../../media/textures/";

		std::unordered_map<std::wstring, int> m_TextureMapIndex;
		std::vector<ComPtr<ID3D11ShaderResourceView>> m_Textures;

	};
}

#endif