#ifndef Camera_H
#define Camera_H

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

namespace byhj
{

namespace d3d
{

const float Pi = 3.1415926535f;

class Camera
{
public:
    Camera():m_Theta(1.5f * Pi), m_Phi(0.25f * Pi), m_Radius(15.0f)
	{
		m_LastMousePos.x = 0;
		m_LastMousePos.y = 0;
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&m_World,  I);
		XMStoreFloat4x4(&m_View ,  I);
		XMStoreFloat4x4(&m_Proj ,  I);
	}

	void update();
	XMFLOAT4X4 GetViewMatrix() const 
	{
		return m_View;
	}
    XMFLOAT4X4 GetProjMatrix() const
	{
		return m_Proj;
	}
	XMFLOAT3 GetPos()
	{
		return pos;
	}
	XMFLOAT3 GetTarget()
	{
		return target;
	}
	XMFLOAT3 GetUp()
	{
		return up;
	}
	void SetRadius(float r)
	{
		m_Radius = r;
	}
	void OnMouseDown(WPARAM btnState, int x, int y, HWND hWnd);
	void OnMouseMove(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseWheel(WPARAM btnState, int x, int y, float aspect);

private:

	float m_Theta;
	float m_Phi;
	float m_Radius;
	POINT m_LastMousePos;

	XMFLOAT4X4 m_World;
	XMFLOAT4X4 m_View;
	XMFLOAT4X4 m_Proj;
	XMFLOAT3 pos;
	XMFLOAT3 target;
	XMFLOAT3 up;
	float m_aspect;
};

}
}

#endif