#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/App.h"
#include "d3d/Utility.h"
#include "d3d/Font.h"
#include "d3d/Timer.h"
#include "d3d/Camera.h"

#include "object.h"

namespace byhj
{

	class RenderSystem : public d3d::App
{
public:
	RenderSystem();
	~RenderSystem();

	void v_Init();
	void v_Update();
	void v_Render();
	void v_Shutdown();

	void UpdateScene();
	void v_OnMouseDown(WPARAM btnState, int x, int y);
	void v_OnMouseMove(WPARAM btnState, int x, int y);
	void v_OnMouseUp(WPARAM btnState, int x, int y);
	void v_OnMouseWheel(WPARAM btnState, int x, int y);

private:
	void init_device();
	void init_camera();
	void init_object();
	void init_fbo();

	void BeginScene();
	void EndScene();
	void TurnZBufferOn();
	void TurnZBufferOff();
	void TurnBlendingOn();
	void TurnBlendingOff();

	void DrawFps();
	void DrawInfo(); 

	byhj::Object m_Fire;
	d3d::Font m_Font;
	d3d::Timer m_Timer;
	d3d::Camera m_Camera;

	float fps = 0.0f;
	int m_videoCardMemory;
	std::wstring m_videoCardInfo;

	ComPtr<IDXGISwapChain>           m_pSwapChain ;    
	ComPtr<ID3D11Device>             m_pD3D11Device;
	ComPtr<ID3D11DeviceContext>      m_pD3D11DeviceContext;
	ComPtr<ID3D11RenderTargetView>   m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView>   m_pDepthStencilView;
	ComPtr<ID3D11Texture2D>          m_pDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState>  m_pDepthStencilState;
	ComPtr<ID3D11DepthStencilState>  m_pDepthDisabledStencilState;
	ComPtr<ID3D11RasterizerState>    m_pRasterState;
	ComPtr<ID3D11BlendState     >    m_pAlphaEnableState;
	ComPtr<ID3D11BlendState     >    m_pAlphaDisableState;
	ComPtr<ID3D11Texture2D>          m_pRttRenderTargetTexture;
	ComPtr<ID3D11RenderTargetView>   m_pRttRenderTargetView;
	ComPtr<ID3D11ShaderResourceView> m_pRttShaderResourceView;

	d3d::MatrixBuffer m_Matrix;
};
}
#endif