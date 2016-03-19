#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "MeshLoad.h"
#include "windowInfo.h"
#include "skybox.h"

#include <memory>
#include <gl/glew.h>

namespace byhj
{
	class OGLRender : public OGLApp
	{
	public:
		OGLRender();
		~OGLRender();

		void v_init();
		void v_update(const glm::mat4 &camMat);
		void v_render();
		void v_shutdown();

	private:
		MeshLoad *m_MeshLoad;
		Skybox   *m_skybox;
	};
}
#endif