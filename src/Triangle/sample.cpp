#include "sample.h"


namespace byhj
{
	Sample::Sample()
	{
		m_pRender = new OGLRender;
	}

	Sample::~Sample()
	{
	}

	void Sample::init()
	{
	}

	void Sample::run()
	{
		
	}

	void Sample::end()
	{
		delete m_pRender;
	}

}