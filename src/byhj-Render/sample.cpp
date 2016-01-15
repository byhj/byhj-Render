#include "sample.h"
#include "ogl/oglRender.h"
#include "d3d/d3dRender.h"
#include "d3d/d3dApp.h"

namespace byhj
{
	Sample::Sample()
	{
		m_pRoot = Root::getInstance();
		m_pApp = new ogl::OGLApp();
	    m_pApp->setRender(new OGLRender() );
		m_pRoot->setApp(m_pApp);
	}

	Sample::~Sample()
	{

	}
    
	void Sample::run()
	{
		m_pRoot->beginScene();

		m_pRoot->endScene();
	}

}