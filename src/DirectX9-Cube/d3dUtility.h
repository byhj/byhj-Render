#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include <d3dx9.h>
#include <windows.h>
#pragma  comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3d9.lib")

namespace byhj
{
	//init the window
	bool init(
		HINSTANCE hInstance,        //application instance
	    int width, int height,      //backe buffer dimensions
		bool windowed,              //full window
		D3DDEVTYPE deviceType,      //HAL or REF
		IDirect3DDevice9 **device) ; //[out] the created device

	// message loop
	int EnterMsgLoop( bool (*ptr_render)(float timeDelta) );

	//window procedure 
	LRESULT CALLBACK WndProc(
		HWND hwnd,
	    UINT msg,
		WPARAM wParam,
		LPARAM lParam
		);

	template<class T>  void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}
	template<class T>  void Delete(T t)
	{
		if (t)
		{
			delete t;
			t = 0;
		}
	}
}
#endif