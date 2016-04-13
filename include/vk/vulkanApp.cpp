#include "vulkanApp.h"

namespace byhj {

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static VulkanApp *pVulkanAppHandle = 0;

 void VulkanApp::v_run()
 {
	 setupWindow();
	 v_init();

	 MSG msg;
	 ZeroMemory(&msg, sizeof(MSG));

	 while (TRUE)
	 {
		 auto tStart = std::chrono::high_resolution_clock::now();
		 if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		 {
			 TranslateMessage(&msg);
			 DispatchMessage(&msg);
		 }
		 if (msg.message == WM_QUIT)
		 {
			 break;
		 }

		 v_update();
		 v_render();

		 auto tEnd = std::chrono::high_resolution_clock::now();
		 auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
		 frameTimer = (float)tDiff / 1000.0f;
		 // Convert to clamped timer value
		 if (!paused)
		 {
			 timer += timerSpeed * frameTimer;
			 if (timer > 1.0)
			 {
				 timer -= 1.0f;
			 }
		 }
	 }

 }

 void VulkanApp::setupWindow() {

	 bool fullscreen = false;
	 pVulkanAppHandle = this;
	 m_hInstance = GetModuleHandle(NULL);

	 WNDCLASSEX wndClass;
	 wndClass.cbSize = sizeof(WNDCLASSEX);
	 wndClass.style = CS_HREDRAW | CS_VREDRAW;
	 wndClass.lpfnWndProc = WndProc;
	 wndClass.cbClsExtra = 0;
	 wndClass.cbWndExtra = 0;
	 wndClass.hInstance = m_hInstance;
	 wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	 wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	 wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	 wndClass.lpszMenuName = NULL;
	 wndClass.lpszClassName = m_name.c_str();
	 wndClass.hIconSm = LoadIcon(NULL, IDI_WINLOGO);

	 if (!RegisterClassEx(&wndClass))
	 {
		 std::cout << "Could not register window class!\n";
		 fflush(stdout);
		 exit(1);
	 }

	 int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	 int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	 if (fullscreen)
	 {
		 DEVMODE dmScreenSettings;
		 memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		 dmScreenSettings.dmSize = sizeof(dmScreenSettings);
		 dmScreenSettings.dmPelsWidth = screenWidth;
		 dmScreenSettings.dmPelsHeight = screenHeight;
		 dmScreenSettings.dmBitsPerPel = 32;
		 dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		 if ((width != screenWidth) && (height != screenHeight))
		 {
			 if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
			 {
				 if (MessageBox(NULL, "Fullscreen Mode not supported!\n Switch to window mode?", "Error", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
				 {
					 fullscreen = FALSE;
				 }
			 }
		 }

	 }

	 DWORD dwExStyle;
	 DWORD dwStyle;

	 if (fullscreen)
	 {
		 dwExStyle = WS_EX_APPWINDOW;
		 dwStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	 }
	 else
	 {
		 dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
		 dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	 }

	 RECT windowRect;
	 if (fullscreen)
	 {
		 windowRect.left = (long)0;
		 windowRect.right = (long)screenWidth;
		 windowRect.top = (long)0;
		 windowRect.bottom = (long)screenHeight;
	 }
	 else
	 {
		 windowRect.left = (long)screenWidth / 2 - width / 2;
		 windowRect.right = (long)width;
		 windowRect.top = (long)screenHeight / 2 - height / 2;
		 windowRect.bottom = (long)height;
	 }

	 AdjustWindowRectEx(&windowRect, dwStyle, FALSE, dwExStyle);

	 m_hWnd = CreateWindowEx(0,
		 m_name.c_str(),
		 m_title.c_str(),
		 //		WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_SYSMENU,
		 dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		 windowRect.left,
		 windowRect.top,
		 windowRect.right,
		 windowRect.bottom,
		 NULL,
		 NULL,
		 m_hInstance,
		 NULL);

	 if (!m_hWnd)
	 {
		 printf("Could not create window!\n");
		 fflush(stdout);
		 exit(1);
	 }

	 RECT   rect;
	 GetClientRect(m_hWnd, &rect);
	 m_clientWidth  = rect.right - rect.left;
	 m_clientHeight  = rect.bottom - rect.top;

	 ShowWindow(m_hWnd, SW_SHOW);
	 SetForegroundWindow(m_hWnd);
	 SetFocus(m_hWnd);
 }
 int VulkanApp::getClientWidth() const
 {
	 return m_clientWidth;

 }
 int  VulkanApp::getClientHeight() const
 {
	 return m_clientHeight;
 }

LRESULT CALLBACK VulkanApp::handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
 {
	 switch (uMsg)
	 {
	 case WM_CHAR:  break;
	 case WM_KEYDOWN: {
		 if (wParam == VK_ESCAPE)
			 PostMessage(m_hWnd, WM_DESTROY, 0, 0);
		 return 0;
	 }break;
	 case WM_LBUTTONDOWN:  break;
	 case WM_LBUTTONUP:    break;
	 default:
		 return DefWindowProc(hWnd, uMsg, wParam, lParam);
	 }
 }

 LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
 {
	 switch (umessage)
	 {
	 case WM_DESTROY: {
		 PostQuitMessage(0);
		 return 0;
	 }
	 case WM_CLOSE: {
		 PostQuitMessage(0);
		 return 0;
	 }
					// All other messages pass to the message handler in the system class.
	 default: {
		 return pVulkanAppHandle->handleMessages(hwnd, umessage, wparam, lparam);
	 }

	 }
 }
}
