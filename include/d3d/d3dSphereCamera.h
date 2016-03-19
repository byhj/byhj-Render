#ifndef D3DSphereCamera_H
#define D3DSphereCamera_H

#include <d3d11.h>
#include <vector>
#include <DirectXMath.h>
#include <memory>

using namespace DirectX;

namespace byhj
{

	class D3DSphereCamera
	{
	public:
		D3DSphereCamera();
		~D3DSphereCamera() = default;

		void init(HWND hWnd);
		void update(UINT uMsg, WPARAM wParam, LPARAM lParam);
		
		XMFLOAT4X4 getViewMat() const;
		XMFLOAT4X4 getProjMat() const;

		XMFLOAT3   getCamPos() const;
		XMFLOAT3   getCamTarget() const;
		XMFLOAT3   getCamUp() const;

		void setRadius(float radius);

		static std::shared_ptr<D3DSphereCamera> getInstance();
		float Clamp(const float& x, const float& low, const float& high);

	private:
		void updateMat();
		void onMouseDown(WPARAM btnState, int x, int y);
		void onMouseMove(WPARAM btnState, int x, int y);
		void onMouseUp(WPARAM btnState, int x, int y);
		void onMouseWheel(WPARAM btnState, int x, int y);

		float m_Theta;
		float m_Phi;
		float m_Radius;
		POINT m_LastMousePos;

		HWND hWnd;

		XMFLOAT4X4 m_World;
		XMFLOAT4X4 m_View;
		XMFLOAT4X4 m_Proj;
		XMFLOAT3 pos;
		XMFLOAT3 target;
		XMFLOAT3 up;
		float m_aspect;

		static std::shared_ptr<D3DSphereCamera> pInstance;
	};

}

#endif