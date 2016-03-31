#ifdef _DEBUG
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif // DEBUG

#ifdef _WIN32
#define _XM_NO_INTRINSICS_
#endif 

#include "RenderSystem.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	auto app= new byhj::RenderSystem;
	app->Run();
	delete app;

	return 0;
}
