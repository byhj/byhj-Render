#ifndef D3D11Render_H
#define D3D11Render_H

#include "d3d/d3dApp.h"
#include "d3d/d3dFont.h"
#include "d3d/d3dCamera.h"

#include <d3d11.h>
#include <wrl.h>

#include "Timer.h"

#include "cube.h"
#include "plane.h"

namespace byhj
{
	class D3D11Render : public D3DApp
	{
	public:
		D3D11Render();
		~D3D11Render();

		void v_init() ;
		void v_update();
		void v_render();
		void v_shutdown();

		void v_OnMouseDown(WPARAM btnState, int x, int y);
		void v_OnMouseMove(WPARAM btnState, int x, int y);
		void v_OnMouseUp(WPARAM btnState, int x, int y);
		void v_OnMouseWheel(WPARAM btnState, int x, int y);

	private:

		void init_device();
		void init_camera();
		void init_object();


		void drawfps();
		void drawInfo();
		D3DFont m_Font;
		D3DCamera m_Camera;

		Timer m_Timer;
		
		Cube m_cube;
		Plane m_plane;

		float fps = 0.0f;
		int m_videoCardMemory;
		std::wstring m_videoCardInfo;
		XMFLOAT4X4 model;
		XMFLOAT4X4 view;
		XMFLOAT4X4 proj;
		IDXGISwapChain         * m_pSwapChain;
		ID3D11Device           * m_pD3D11Device;
		ID3D11DeviceContext    * m_pD3D11DeviceContext;
		ID3D11RenderTargetView * m_pRenderTargetView;
		ID3D11DepthStencilView * m_pDepthStencilView;
		ID3D11Texture2D        * m_pDepthStencilBuffer;
		ID3D11RasterizerState  * m_pRasterState;

	};
}
#endif