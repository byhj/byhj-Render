#ifndef MeshLoad_H
#define MeshLoad_H

#include <d3d11.h>

#include "d3d/d3dShader.h"
#include "d3d/D3DUtility.h"

namespace byhj {

	class MeshLoad {
	public:
          void init();
		  void update();
		  void render();
		  void shutdown();

	private:
		void init_buffer();
		void init_texture();
		void init_shader();

		D3DShader m_meshShader;
	 
		ID3D11InputLayout 
	};
}
#endif