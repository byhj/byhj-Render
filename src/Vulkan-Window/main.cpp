#include "base.h"

int APIENTRY WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	                  _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
{
	MSG msg;
	bool done;
	Base base;
	base.init();
	base.createWindow();
	base.prepare();
	base.render();
	base.shutdown();

	 return (int)msg.wParam;
}