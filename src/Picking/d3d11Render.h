#ifndef D3D11Render_H
#define D3D11Render_H

#include "d3d/d3dApp.h"
#include "d3d/d3dFont.h"
#include "d3d/d3dCamera.h"

#include <d3d11.h>
#include <wrl.h>
#include "Timer.h"
#include "cube.h"
#include "Plane.h"
#include "d3dPicking.h"

namespace byhj
{
	class D3D11Render : public D3DApp
	{
	public:
		D3D11Render();
		~D3D11Render();

		void v_init()     override;
		void v_update()   override;
		void v_render()   override;
		void v_shutdown() override;

		void v_onMouseDown(WPARAM btnState, int x, int y)  override;
		void v_onMouseMove(WPARAM btnState, int x, int y)  override;
		void v_onMouseUp(WPARAM btnState, int x, int y)    override;
		void v_onMouseWheel(WPARAM btnState, int x, int y) override;

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
		D3DPicking m_picking;

		float fps = 0.0f;
		int m_videoCardMemory;
		std::wstring m_videoCardInfo;
		D3DMVPMatrix m_matrix;

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