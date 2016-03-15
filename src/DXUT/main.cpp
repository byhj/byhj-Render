#ifdef _DEBUG
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#include "RenderSystem.h"
#include "DxutHelper.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	// Enable run-time memory check for debug builds.
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	byhj::DXUTHelper dxutHelper;

	dxutHelper.init();
	dxutHelper.render();
	dxutHelper.shutdown();

	return 0;
}