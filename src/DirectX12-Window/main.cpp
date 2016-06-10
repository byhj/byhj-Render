#include "d3d12Render.h"
#include "root.h"

// #ifdef _DEBUG
// #include <vld.h>
// #endif

int main()
{
	auto D3DApp = std::make_shared<byhj::D3D12Render>();
	byhj::Root::getInstance()->setRender(D3DApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}