#ifndef OGLRender_H
#define OGLRender_H

#include "ogl/oglApp.h"
#include "cube.h"

namespace byhj
{

class OGLRender : public OGLApp
{
public:
	OGLRender() {}
	~OGLRender() {}

	///////////////////////// Triangle and Render //////////////////////////////////
	void v_init();
	void v_update();
	void v_render();
	void v_shutdown();

private:


	byhj::Cube   m_cube;

};


}

#endif