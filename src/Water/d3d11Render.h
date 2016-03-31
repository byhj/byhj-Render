#ifndef D3D11Render_H
#define D3D11Render_H

#include "d3d/d3dApp.h"
#include "d3d/d3dFWFont.h"

#include <d3d11.h>
#include <wrl.h>
#include "Timer.h"
#include "d3d/d3dSphereCamera.h"

#include "bath.h"
#include "ground.h"
#include "wall.h"
#include "water.h"
#include "lightShader.h"
#include "waterShader.h"
#include "refractShader.h"

namespace byhj
{
	class D3D11Render : public D3DApp
	{
	public:
		D3D11render();
		~D3D11render();

		void v_init()     override;
		void v_update()   override;
		void v_render()   override;
		void v_shutdown() override;
	private:

		void init_device();
		void init_camera();
		void init_object();


		void drawfps();
		void drawInfo();

		D3DFWFont m_Font;
		Timer m_Timer;

	    Bath   bathModel;
	    Ground groundModel;
	    Wall   wallModel;
	    Water  waterModel;
	    
	    LightShader m_LightShader;
	    WaterShader m_WaterShader;
	    RefractShader m_RefracShader;

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
		ComPtr<ID3D11DepthStencilState>  m_pDepthStencilState;
		ComPtr<ID3D11DepthStencilState>  m_pDepthDisabledStencilState;
		ComPtr<ID3D11Texture2D         > m_pReflectRTT;
		ComPtr<ID3D11Texture2D         > m_pRefractRTT;
		ComPtr<ID3D11RenderTargetView  > m_pReflectRTV;
		ComPtr<ID3D11RenderTargetView  > m_pRefractRTV;
		ComPtr<ID3D11ShaderResourceView> m_pReflectSRV;
		ComPtr<ID3D11ShaderResourceView> m_pRefractSRV;
	};
}
#endif