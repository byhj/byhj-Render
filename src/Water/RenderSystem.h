#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/App.h"
#include "d3d/Utility.h"
#include "d3d/Font.h"
#include "d3d/Timer.h"
#include "d3d/Camera.h"
#include "d3d/RTT.h"

#include "bath.h"
#include "ground.h"
#include "wall.h"
#include "water.h"
#include "lightShader.h"
#include "waterShader.h"
#include "refractShader.h"
#include "TextureMgr.h"

namespace byhj
{

	class RenderSystem : public d3d::App
{
public:
	RenderSystem();
	~RenderSystem();

	void v_init();
	void v_update();
	void v_render();
	void v_shutdown();

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
	void DrawFps();
	void DrawInfo(); 

	byhj::Bath   bathModel;
	byhj::Ground groundModel;
	byhj::Wall   wallModel;
	byhj::Water  waterModel;

	byhj::LightShader m_LightShader;
	byhj::WaterShader m_WaterShader;
	byhj::RefractShader m_RefracShader;

	byhj::TextureMgr m_TextureMgr;

	d3d::Font m_Font;
	d3d::Timer m_Timer;
	d3d::Camera m_Camera;
	d3d::RTT    m_Rtt;
	float fps = 0.0f;
	int m_videoCardMemory;
	std::wstring m_videoCardInfo;

	ComPtr<IDXGISwapChain> m_pSwapChain ;    
	ComPtr<ID3D11Device>   m_pD3D11Device;
	ComPtr<ID3D11DeviceContext> m_pD3D11DeviceContext;
	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;
	ComPtr<ID3D11DepthStencilView> m_pDepthStencilView;
	ComPtr<ID3D11Texture2D> m_pDepthStencilBuffer;
	ComPtr<ID3D11DepthStencilState>  m_pDepthStencilState;
	ComPtr<ID3D11DepthStencilState>  m_pDepthDisabledStencilState;
	ComPtr<ID3D11RasterizerState> m_pRasterState;

	ComPtr<ID3D11Texture2D         > m_pReflectRTT;
	ComPtr<ID3D11Texture2D         > m_pRefractRTT;
	ComPtr<ID3D11RenderTargetView  > m_pReflectRTV;
	ComPtr<ID3D11RenderTargetView  > m_pRefractRTV;
	ComPtr<ID3D11ShaderResourceView> m_pReflectSRV;
	ComPtr<ID3D11ShaderResourceView> m_pRefractSRV;

	D3DMVPMatrix m_Matrix;
};
}
#endif