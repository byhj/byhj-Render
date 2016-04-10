#include "oglRender.h"
#include "d3d11Render.h"

#include "root.h"

// #ifdef _DEBUG
// #include <vld.h>
// #endif

int main()
{
	auto oglApp = std::make_shared<byhj::OGLRender>();
	auto d3dApp = std::make_shared<byhj::D3D11Render>();
	byhj::Root::getInstance()->setRender(oglApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}