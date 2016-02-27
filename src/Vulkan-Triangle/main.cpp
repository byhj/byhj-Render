#include <windows.h>

#pragma comment(lib, "vulkan-1.lib")

#ifdef _WIN32

//Enable the console to output the debug message
#ifdef _DEBUG
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

int APIENTRY WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	                  _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
#else
int main(int argc, char **argv)
#endif

{

	return 0;
}