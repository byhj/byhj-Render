#ifndef EffectHelper_H
#define EffectHelper_H

#include "d3dx11effect.h"
#include "d3dxGlobal.h"
#include <vector>
#include <d3dcompiler.h>

namespace byhj
{
	class EffectHelper {
	public:
		EffectHelper();
		~EffectHelper();

		void init(std::string effectFile);
		void render();
		void shutdown();

	private:
	    ID3DX11Effect *m_pEffect = nullptr;
		ID3DX11EffectTechnique *m_pEffectTechnique = nullptr;
		std::vector<ID3DX11EffectMatrixVariable*> m_vEffectMat;
		std::vector<ID3DX11EffectScalarVariable*> m_vEffectScalar;
		std::vector<ID3DX11EffectShaderResourceVariable*> m_vEffectSRV;
	};
}

#endif