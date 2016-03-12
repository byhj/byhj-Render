#include "d3d11Render.h"
#include "root.h"


#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

int main()
{
	auto d3dApp = std::make_shared<byhj::D3D11Render>();
	byhj::Root::getInstance()->setRender(d3dApp);
	byhj::Root::getInstance()->BeginScene();
	byhj::Root::getInstance()->EndScene();

	return 0;
}