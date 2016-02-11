#include "oglRender.h"
#include "root.h"
#include <memory>

int main(int argc, char **argv)
{
	auto oglApp = std::make_shared<byhj::OGLRender>();
	byhj::Root::getInstance()->setRender(oglApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}