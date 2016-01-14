#ifndef D3DCAMERA_H
#define D3DCAMERA_H

#include <windows.h>
#include <d3dx11.h>
#include <dinput.h>
#include <d3d11.h>
#include <xnamath.h>

namespace byhj
{

namespace d3d
{

class Camera
{
public:
	Camera()
	{
		moveLeftRight   = 0.0f;
		moveBackForward = 0.0f;
		camYaw = 0.0f;
		camPitch = 0.0f;
		rot = 0.01f;
		rightMouseClicked = false;
	}
	bool Init(HINSTANCE hInstance , HWND hWnd);
	void DetectInput(double time , HWND hWnd);
	void UpdateCamera();

	XMFLOAT4X4 GetViewMatrix();
	XMFLOAT4   GetCamPos()	  ;
	XMFLOAT4   GetCamTarget()  ;
	float GetMouseX();
	float GetMouseY();
	bool GetRightMouseClicked();

private:
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
};

}
}

#endif