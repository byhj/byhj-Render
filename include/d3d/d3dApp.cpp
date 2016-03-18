#include "D3DApp.h"
#include "windowInfo.h"
#include <cegui/CEGUI.h>

namespace byhj
{

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static D3DApp *D3DAppHandle = 0;

D3DApp::D3DApp()
{

}
D3DApp::~D3DApp()
{

}
void D3DApp::v_run()
{
	bool ret = init_window();
	v_init();
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (ret)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		if (msg.message == WM_QUIT) {
			ret = false;
		}
		else
		{
			v_update();
			v_render();
			ret = true;
		}

	}
	v_shutdown();

}

bool D3DApp::init_window()
{
	//Set the window in the middle of screen
	float ScreenWidth = WindowInfo::getInstance()->getWidth();
	float ScreenHeight = WindowInfo::getInstance()->getHeight();
	float PosX = WindowInfo::getInstance()->getPosX() - 150;
	float PosY = WindowInfo::getInstance()->getPosY() - 100;
	float ScreenNear = 0.1f;
	float ScreenFar  = 1000.0f;

	D3DAppHandle = this;
	m_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX wc;	
	wc.cbSize = sizeof(WNDCLASSEX);	
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;	
	wc.cbClsExtra = NULL;	
	wc.cbWndExtra = NULL;	
	wc.hInstance = m_hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);	
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);	
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 3);
	wc.lpszMenuName = NULL;	
	wc.lpszClassName = m_WndClassName;	
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); 

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Registering Class Fail",	L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	//Create the window and show

	m_hWnd = CreateWindowEx(	
		NULL,	           
		m_WndClassName,
		m_AppName,
		WS_OVERLAPPEDWINDOW,	
		PosX, PosY,
		ScreenWidth,	
		ScreenHeight,	
		NULL,
		NULL,
		m_hInstance,	
		NULL
		);

	if (!m_hWnd )	
	{
		MessageBox(NULL, L"Creating Window Failed", L"Error", MB_OK | MB_ICONERROR);
		return 1;
	}

	v_setOGL();

	RECT   rect;
	GetClientRect(m_hWnd, &rect);
	m_clientWidth  = rect.right - rect.left;
	m_clientHeight  = rect.bottom - rect.top;

	ShowWindow(m_hWnd, SW_SHOW);
	SetForegroundWindow(m_hWnd);
	SetFocus(m_hWnd);

	return true;
}

int D3DApp::getClientWidth() const 
{
	return m_clientWidth;

}
int D3DApp::getClientHeight() const
{
	return m_clientHeight;
}
HINSTANCE D3DApp::getAppInst() const 
{ 
	return m_hInstance; 
}

HWND  D3DApp::getHwnd()  const 
{
	return m_hWnd;
}

LRESULT CALLBACK D3DApp::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
#ifdef USE_CEGUI
	D3DCEGUI::getInstance()->setCallback(uMsg, wParam, lParam);
#endif
	switch (uMsg)
	{
	case WM_CHAR:  break;
	case WM_KEYDOWN: {
			if(wParam == VK_ESCAPE)
				PostMessage(m_hWnd, WM_DESTROY, 0, 0);
			return 0;
	}
    case WM_LBUTTONDOWN:  break;
    case WM_RBUTTONDOWN: v_onMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
    case WM_LBUTTONUP:    break;
	case WM_RBUTTONUP: v_onMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));    break;
    case WM_MOUSEMOVE: v_onMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));  break;
    case WM_MOUSEWHEEL: v_onMouseWheel(wParam, GET_WHEEL_DELTA_WPARAM(wParam), GET_Y_LPARAM(lParam)); break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	} 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
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
		return D3DAppHandle->MessageHandler(hwnd, umessage, wparam, lparam);
	}

	}
}


}