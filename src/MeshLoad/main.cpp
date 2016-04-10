#include "oglRender.h"

#include "root.h"

// #ifdef _DEBUG
// #include <vld.h>
// #endif

int main()
{
	auto oglApp = std::make_shared<byhj::OGLRender>();
	byhj::Root::getInstance()->setRender(oglApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}