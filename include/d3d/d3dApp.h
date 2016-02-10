#ifndef APP_H
#define APP_H

//use window sdk, not the mfc sdk parts;
#define WIN32_LEAN_AND_MEAN


#include <WindowsX.h>
#include <windows.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>

#include <string>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>

#include "d3d/Utility.h"
#include "app.h"
#include "d3dRender.h"


#include <DirectXMath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

namespace byhj
{

class D3DApp : public App
{
public:
	D3DApp()  ;
	~D3DApp() ;

	void v_run() override;
	virtual void v_init()     {}
	virtual void v_update()   {}
	virtual void v_render()   {}
	virtual void v_shutdown() {}
	virtual void v_setOGL() {}

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);


	// Convenience overrides for handling mouse input.
	virtual void v_OnMouseDown(WPARAM btnState, int x, int y)  { }
	virtual void v_OnMouseUp(WPARAM btnState, int x, int y)    { }
	virtual void v_OnMouseMove(WPARAM btnState, int x, int y)  { }
	virtual void v_OnMouseWheel(WPARAM btnState, int x, int y) { }

protected:
	int   m_ScreenWidth;
	int   m_ScreenHeight;
	float m_ScreenFar;
	float m_ScreenNear;
	int   m_PosX;
	int   m_PosY;

	LPCTSTR m_AppName      = L"DirectX:";
	LPCTSTR m_WndClassName = L"Window";

	//void      GetVideoCardInfo(char &, int &);
	HINSTANCE getAppInst() const { return m_hInstance; }
	HWND      getHwnd()    const { return m_hWnd; }


private:

	bool init_window();

	HINSTANCE  m_hInstance;
	HWND       m_hWnd;
};


}
#endif