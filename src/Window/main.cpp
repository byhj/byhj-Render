#include "oglRender.h"
#include "d3d11Render.h"
#include "d3d12Render.h"
#include "oglWin32App.h"
#include "root.h"

// #ifdef _DEBUG
// #include <vld.h>
// #endif

int main()
{
	auto d3dApp      = std::make_shared<byhj::D3D11Render>();
	auto d3d12App    = std::make_shared<byhj::D3D12Render>();
	auto oglApp      = std::make_shared<byhj::OGLRender>();
	auto oglWin32App = std::make_shared<byhj::OGLWIN32Render>();

	byhj::Root::getInstance()->setRender(d3dApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}