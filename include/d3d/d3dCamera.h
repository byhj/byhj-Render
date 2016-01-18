#ifndef D3DD3DCamera_H
#define D3DD3DCamera_H

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>

using namespace DirectX;

namespace byhj
{

class D3DCamera
{
public:
    D3DCamera();
	virtual ~D3DCamera();
	void update();

	XMFLOAT4X4 getViewMat() const;
    XMFLOAT4X4 getProjMat() const;
	XMFLOAT3   getPos() const;
	XMFLOAT3   getTarget() const;
	XMFLOAT3   getUp() const;

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

#endif