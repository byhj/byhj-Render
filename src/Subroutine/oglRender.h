#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "Plane.h"

namespace byhj {
	class OGLRender : public OGLApp {
	public:
		OGLRender() = default;
		~OGLRender() = default;

		void v_init() override;
		void v_update() override;
		void v_render() override;
		void v_shutdown() override;

	private:
		Plane m_plane;
	};
}
#endif