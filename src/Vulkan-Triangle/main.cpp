#include "Triangle.h"
#include <memory>

#pragma comment(lib, "vulkan-1.lib")
#pragma comment(lib, "assimp")
#pragma comment(lib, "glew32")

static std::shared_ptr<Triangle> pTriangle = std::make_shared<Triangle>();

#ifdef _WIN32

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (pTriangle != NULL)
	{
		pTriangle->handleMessages(hWnd, uMsg, wParam, lParam);
	}
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));
}

#else 

static void handleEvent(const xcb_generic_event_t *event)
{
	if (vulkanExample != NULL)
	{
		vulkanExample->handleEvent(event);
	}
}
#endif

//Enable the console to output the debug message
#ifdef _DEBUG
#pragma comment( linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif

#ifdef _WIN32
int APIENTRY WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance,
	                  _In_ LPSTR lpCmdLine, _In_ int nShowCmd )
#else
int main(int argc, char **argv)
#endif

{
	pTriangle->setupWindow(hInstance, WndProc);
	pTriangle->initSwapchain();
	pTriangle->init();
	pTriangle->renderLoop();

	return 0;
}