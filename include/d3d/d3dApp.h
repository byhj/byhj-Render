#ifndef _D3DAPP_H
#define _D3DAPP_H

//use window sdk, not the mfc sdk parts;
#define WIN32_LEAN_AND_MEAN


#include <WindowsX.h>
#include <windows.h>

#include <string>
#include <ctime>
#include <algorithm>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <memory>

#include "app.h"
#include "d3d/d3dUtility.h"

#define USE_CAMERA

#ifdef USE_CEGUI
#include "d3d/d3dCEGUI.h"
#endif

#ifdef USE_CAMERA
#include "d3d/d3dSphereCamera.h"
#include "d3d/d3dEulerCamera.h"
#endif

using namespace DirectX;
using namespace Microsoft::WRL;

namespace byhj
{

class D3DApp : public App
{
public:
	D3DApp()  ;
	~D3DApp() ;

	void v_run()   ;
	virtual void v_init()     = 0;
	virtual void v_update()   = 0;
	virtual void v_render()   = 0;
	virtual void v_shutdown() = 0;

	virtual void v_setOGL() {};

	LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		
	int getClientWidth() const;
	int getClientHeight() const;

protected:

	LPCTSTR m_AppName      = L"DirectX:";
	LPCTSTR m_WndClassName = L"Window";

	HINSTANCE getAppInst() const;
	HWND      getHwnd()    const;


private:

	bool init_window();
	int m_clientWidth;
	int m_clientHeight;

	HINSTANCE  m_hInstance;
	HWND       m_hWnd;
};


}
#endif