#include "SlideCamera.h"

#define SHADER_DEBUG

namespace byhj
{


bool SlideCamera::Init(HINSTANCE hInstance, HWND hWnd)
{

	HRESULT hr;
	hr = DirectInput8Create(hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&DirectInput,
		NULL); 

	hr = DirectInput->CreateDevice(GUID_SysKeyboard, &m_pDIKeyboard, NULL);
	hr = DirectInput->CreateDevice(GUID_SysMouse, &m_pDIMouse, NULL);

	hr = m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pDIMouse->SetDataFormat(&c_dfDIMouse);

#ifdef SHADER_DEBUG
	hr = m_pDIKeyboard->SetCooperativeLevel(hWnd,DISCL_FOREGROUND);
	hr = m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_FOREGROUND);
#else
	hr = m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	hr = m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);
#endif
	return true;
}

void SlideCamera::DetectInput(double time, HWND hWnd)
{
	DIMOUSESTATE mouseCurrState;

	BYTE keyboardState[256];

	//Acquire current mouse and key state
	m_pDIKeyboard->Acquire();
	m_pDIMouse->Acquire();
	m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseCurrState);
	m_pDIKeyboard->GetDeviceState(sizeof(keyboardState),(LPVOID)&keyboardState);

	if(keyboardState[DIK_ESCAPE] & 0x80)
		PostMessage(hWnd, WM_DESTROY, 0, 0);

	float speed = 15.0f * time;

	//Enter ADSW to move SlideCamera left right back forword

	if(keyboardState[DIK_A] & 0x80)
	{
		moveLeftRight -= speed;
	}
	if(keyboardState[DIK_D] & 0x80)
	{
		moveLeftRight += speed;
	}
	if(keyboardState[DIK_W] & 0x80)
	{
		moveBackForward += speed;
	}
	if(keyboardState[DIK_S] & 0x80)
	{
		moveBackForward -= speed;
	}
	//Use mouse to change the rotation matrix
	if((mouseCurrState.lX != mouseLastState.lX) || (mouseCurrState.lY != mouseLastState.lY))
	{
		camYaw += mouseCurrState.lX * 0.001f;
		camPitch += mouseCurrState.lY * 0.001f;

		mouseLastState = mouseCurrState;
	}
	if(mouseCurrState.rgbButtons[0])
	{
		//Get mouse pos
		POINT mousePos;
		GetCursorPos(&mousePos);			
		ScreenToClient(hWnd, &mousePos);
		m_mouseX = mousePos.x;
		m_mouseY = mousePos.y;
		rightMouseClicked = true;
	}

	if(!mouseCurrState.rgbButtons[0])
	{
		rightMouseClicked = false;
	}
	//UpdateSlideCamera();

	return;
}

void SlideCamera::UpdateSlideCamera(std::vector<XMFLOAT3> collidableGeometryPositions,
	std::vector<DWORD> collidableGeometryIndices)
{	
	static XMVECTOR DefaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static XMVECTOR DefaultRight   = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	static XMVECTOR camForward     = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	static XMVECTOR camRight       = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

	static XMVECTOR camUp  = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	static XMVECTOR camPosition    = XMVectorSet(0.0f, 2.0f, -5.0f, 0.0f);
	static XMVECTOR camTarget      = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	static XMMATRIX camView;

	//Rotating the SlideCamera by euler angle
	XMMATRIX camRotationMatrix = XMMatrixRotationRollPitchYaw(camPitch, camYaw, 0);
	camTarget         = XMVector3TransformCoord(DefaultForward, camRotationMatrix );
	camTarget         = XMVector3Normalize(camTarget);

	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(camYaw);

	//We only move the xz plane by axis y
	//Update the SlideCamera vector
	camRight   = XMVector3TransformCoord(DefaultRight, RotateYTempMatrix);
	camUp      = XMVector3TransformCoord(camUp, RotateYTempMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, RotateYTempMatrix);

	CollisionPacket SlideCameraCP;
	SlideCameraCP.ellipsoidSpace = XMVectorSet(1.0f, 3.0f, 1.0f, 0.0f);
	SlideCameraCP.w_Position = camPosition;
	SlideCameraCP.w_Velocity = (moveLeftRight*camRight) + (moveBackForward*camForward);

	camPosition = CollisionSlide(SlideCameraCP,
		collidableGeometryPositions,
		collidableGeometryIndices);

	/*
	// Free-Look SlideCamera
	camRight = XMVector3TransformCoord(DefaultRight, camRotationMatrix);
	camForward = XMVector3TransformCoord(DefaultForward, camRotationMatrix);
	camUp = XMVector3Cross(camForward, camRight);

	//Moving the SlideCamera
	camPosition += moveLeftRight*camRight;
	camPosition += moveBackForward*camForward;
	*/
	moveLeftRight = 0.0f;
	moveBackForward = 0.0f;

	camTarget = camPosition + camTarget;	

	//Set the SlideCamera matrix
	camView = XMMatrixLookAtLH( camPosition, camTarget, camUp );
	XMStoreFloat4x4(&m_camView, XMMatrixTranspose(camView));
}

XMFLOAT4X4  SlideCamera::GetViewMatrix()
{
	return m_camView;
}

XMFLOAT4  SlideCamera::GetCamPos()
{
	//XMStoreFloat4(&m_camPosition, camPosition);
	return m_camPosition;
}

XMFLOAT4   SlideCamera::GetCamTarget()
{
	//XMStoreFloat4(&m_camTarget, camTarget);
	return m_camTarget;
}
float  SlideCamera::GetMouseX()
{
	return m_mouseX;
}
float  SlideCamera::GetMouseY()
{
	return m_mouseY;
}
bool  SlideCamera::GetRightMouseClicked()
{
	return rightMouseClicked;
}

}