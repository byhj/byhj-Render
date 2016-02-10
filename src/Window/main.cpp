#include "d3d11Render.h"
#include "d3d12Render.h"
#include "oglWin32App.h"
#include "root.h"

// #ifdef _DEBUG
// #include <vld.h>
// #endif

int main()
{
	auto oglApp = std::make_shared<byhj::OGLWIN32Render>();
	//auto d3dApp = std::make_shared<byhj::D3D11Render>();
	byhj::Root::getInstance()->setRender(oglApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}