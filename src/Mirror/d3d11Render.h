#ifndef D3D11Render_H
#define D3D11Render_H

#include "d3d/d3dApp.h"
#include "d3d/d3dFWFont.h"


#include <d3d11.h>
#include <wrl.h>

#include "Timer.h"
#include "Cube.h"
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

	private:

		void init_device();
		void init_camera();
		void init_object();

		void TurnZBufferOn();
		void TurnZBufferOff();
		void TurnBlendingOn();
		void TurnBlendingOff();

		void drawfps();
		void drawInfo();
		void init_fbo();
		D3DFWFont m_Font;
		Timer m_Timer;
		byhj::Cube m_Cube;
		byhj::Plane m_Plane;

		float fps = 0.0f;
		int m_videoCardMemory;
		std::wstring m_videoCardInfo;
		D3DMVPMatrix m_matrix;

		IDXGISwapChain          * m_pSwapChain;
		ID3D11Device            * m_pD3D11Device;
		ID3D11DeviceContext     * m_pD3D11DeviceContext;
		ID3D11RenderTargetView  * m_pRenderTargetView;
		ID3D11DepthStencilView  * m_pDepthStencilView;
		ID3D11Texture2D         * m_pDepthStencilBuffer;
		ID3D11RasterizerState   * m_pRasterState;
		ID3D11DepthStencilState * m_pDepthStencilState;
		ID3D11DepthStencilState * m_pDepthDisabledStencilState;
		ID3D11BlendState        * m_pAlphaEnableState;
		ID3D11BlendState        * m_pAlphaDisableState;

		ComPtr<ID3D11Texture2D          > m_pRttRenderTargetTexture;
		ComPtr<ID3D11RenderTargetView   > m_pRttRenderTargetView;
		ComPtr<ID3D11ShaderResourceView > m_pRttShaderResourceView;
	};
}
#endif