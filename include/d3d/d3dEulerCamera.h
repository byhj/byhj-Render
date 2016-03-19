#ifndef D3DEulerCamera_H
#define D3DEulerCamera_H

#include <windows.h>
#include <dinput.h>
#include <d3d11.h>
#include <memory>

#include <DirectXMath.h> 
using namespace DirectX;

#pragma comment(lib, "dinput8")

namespace byhj
{

class D3DEulerCamera
{
public:
	D3DEulerCamera();

	void init(HINSTANCE hInstance , HWND hWnd);
	void detectInput(double time , HWND hWnd);


	XMFLOAT4X4 getViewMat();
	XMFLOAT4   getCamPos();
	XMFLOAT4   getCamTarget();

	float getMouseX();
	float getMouseY();

	bool getRightMouseClicked();

	static std::shared_ptr<D3DEulerCamera> getInstance();

private:
	void update();

	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;
	DIMOUSESTATE mouseLastState;
	LPDIRECTINPUT8 DirectInput;

	XMFLOAT4   m_camTarget;
	XMFLOAT4   m_camPosition;
	XMFLOAT4X4 m_camView;

	float rot;
	float moveLeftRight  ;
	float moveBackForward;

	float camYaw   ;
	float camPitch ;

	float m_mouseX;
	float m_mouseY;
	bool rightMouseClicked;

	static std::shared_ptr<D3DEulerCamera> pInstance;
};

}

#endif