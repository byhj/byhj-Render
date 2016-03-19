#include "D3DSphereCamera.h"
#include "windowInfo.h"
#include "windowsX.h"


namespace byhj
{
	std::shared_ptr<D3DSphereCamera> D3DSphereCamera::pInstance = std::make_shared<D3DSphereCamera>();

	std::shared_ptr<D3DSphereCamera> D3DSphereCamera::getInstance()
	{
		return pInstance;
	}
	D3DSphereCamera::D3DSphereCamera()
		:m_Theta(1.5f * XM_PI), m_Phi(0.25f * XM_PI), m_Radius(5.0f), m_aspect(1.5f)
	{
		m_LastMousePos.x = 0;
		m_LastMousePos.y = 0;
		XMMATRIX I = XMMatrixIdentity();
		XMStoreFloat4x4(&m_World, I);
		XMStoreFloat4x4(&m_View, I);
		XMStoreFloat4x4(&m_Proj, I);
	}

	float D3DSphereCamera::Clamp(const float& x, const float& low, const float& high)
	{
		return x < low ? low : (x > high ? high : x);
	}

	void D3DSphereCamera::init(HWND hWnd) {

		this->hWnd = hWnd;
	}

	void D3DSphereCamera::update(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_RBUTTONDOWN: onMouseDown(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); break;
		case WM_RBUTTONUP: onMouseUp(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));    break;
		case WM_MOUSEMOVE: onMouseMove(wParam, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));  break;
		case WM_MOUSEWHEEL: onMouseWheel(wParam, GET_WHEEL_DELTA_WPARAM(wParam), GET_Y_LPARAM(lParam)); break;
		}

		updateMat();
	}

	void D3DSphereCamera::updateMat()
	{
		// Convert Spherical to Cartesian coordinates.
		float x = m_Radius * sinf(m_Phi) * cosf(m_Theta);
		float z = m_Radius * sinf(m_Phi) * sinf(m_Theta);
		float y = m_Radius * cosf(m_Phi);

		// Build the view matrix.
		XMVECTOR pos    = XMVectorSet(x, y, z, 1.0f);
		XMVECTOR target = XMVectorZero();
		XMVECTOR up     = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		XMStoreFloat3(&this->pos, pos);
		XMStoreFloat3(&this->target, target);
		XMStoreFloat3(&this->up, up);
		XMMATRIX View = XMMatrixLookAtLH(pos, target, up);
		XMMATRIX Proj  = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_aspect, 1.0f, 1000.0f);

		XMStoreFloat4x4(&m_View, XMMatrixTranspose(View));
		XMStoreFloat4x4(&m_Proj, XMMatrixTranspose(Proj));

	}
	void D3DSphereCamera::onMouseWheel(WPARAM btnState, int x, int y)
	{
		static float zoom = 45.0f;
		zoom += x * 0.01f;
		XMMATRIX Proj  = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), m_aspect, 1.0f, 1000.0f);
		XMStoreFloat4x4(&m_Proj, XMMatrixTranspose(Proj));
	}

	void D3DSphereCamera::onMouseDown(WPARAM btnState, int x, int y)
	{
		m_LastMousePos.x = x;
		m_LastMousePos.y = y;

		SetCapture(hWnd);
	}

	void D3DSphereCamera::onMouseUp(WPARAM btnState, int x, int y)
	{
		ReleaseCapture();
	}

	void D3DSphereCamera::onMouseMove(WPARAM btnState, int x, int y)
	{
		if ((btnState & MK_LBUTTON) != 0)
		{
			// Make each pixel correspond to a quarter of a degree.
			float dx = XMConvertToRadians(0.25f*static_cast<float>(x - m_LastMousePos.x));
			float dy = XMConvertToRadians(0.25f*static_cast<float>(y - m_LastMousePos.y));

			// Update angles based on input to orbit D3DSphereCamera around box.
			m_Theta += dx;
			m_Phi   += dy;

			// Restrict the angle mPhi.
			m_Phi = Clamp(m_Phi, 0.1f, XM_PI - 0.1f);
		}

		else if ((btnState & MK_RBUTTON) != 0)
		{
			// Make each pixel correspond to 0.005 unit in the scene.
			float dx = 0.005f*static_cast<float>(x - m_LastMousePos.x);
			float dy = 0.005f*static_cast<float>(y - m_LastMousePos.y);

			// Update the D3DSphereCamera radius based on input.
			m_Radius += dx - dy;

			// Restrict the radius.
			m_Radius = Clamp(m_Radius, 3.0f, 15.0f);
		}

		m_LastMousePos.x = x;
		m_LastMousePos.y = y;
	}

	XMFLOAT4X4 D3DSphereCamera::getViewMat() const
	{
		return m_View;
	}
	XMFLOAT4X4 D3DSphereCamera::getProjMat() const
	{
		return m_Proj;
	}
	XMFLOAT3 D3DSphereCamera::getCamPos() const
	{
		return pos;
	}
	XMFLOAT3 D3DSphereCamera::getCamTarget() const
	{
		return target;
	}
	XMFLOAT3 D3DSphereCamera::getCamUp() const
	{
		return up;
	}

}