#include "oglRender.h"

namespace byhj
{
	OGLRender::OGLRender()
	{


	}

	OGLRender::~OGLRender()
	{

	}

	void OGLRender::v_init()
	{
		SoundMgr::getInstance()->load("test.mp3");
		SoundMgr::getInstance()->play("test.mp3");
	}

	void OGLRender::v_update()
	{

	}

	void OGLRender::v_render()
	{
		GLfloat bgColor[] ={ 0.0f, 0.0f, 0.0f, 0.0f };
		glClearBufferfv(GL_COLOR, 0, bgColor);
	}

	void OGLRender::v_shutdown()
	{
		SoundMgr::getInstance()->shutdown();
	}
}