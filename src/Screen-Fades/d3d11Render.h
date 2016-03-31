#ifndef D3D11Render_H
#define D3D11Render_H

#include "d3d/d3dApp.h"
#include "d3d/d3dUtility.h"
#include "d3d/d3dFWFont.h"
#include "d3d/d3dEulerCamera.h"

#include "Timer.h"
#include "Cube.h"
#include "Fade.h"

namespace byhj
{

	class D3D11Render : public D3DApp
{
public:
	D3D11Render();
	~D3D11Render();

	void v_init();
	void v_update();
	void v_render();
	void v_shutdown();

private:
	void init_device();
	void init_camera();
	void init_object();
	void init_fbo();

	void BeginScene();
	void EndScene();
	void TurnZBufferOn();
	void TurnZBufferOff();

	void drawfps();
	void drawinfo(); 

	byhj::Cube m_Cube;
	byhj::Fade m_Fade;
	D3DFWFont m_Font;
	Timer m_Timer;

	float fps = 0.0f;
	int m_videoCardMemory;
	std::wstring m_videoCardInfo;

	ComPtr<IDXGISwapChain> m_pSwapChain ;    
	ComPtr<ID3D11Device>   m_pD3D11Device;
	ComPtr<ID3D11DeviceContext> m_pD3D11DeviceContext;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
	ComPtr<ID3D11DepthStencilState> m_pDepthDisabledStencilState;
	ComPtr<ID3D11RasterizerState> m_pRasterState;

	ComPtr<ID3D11Texture2D> m_pRttRenderTargetTexture;
	ComPtr<ID3D11RenderTargetView> m_pRttRenderTargetView;
	ComPtr<ID3D11ShaderResourceView> m_pRttShaderResourceView;

	D3DMVPMatrix m_Matrix;
};
}
#endif