#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "d3d/App.h"
#include "d3d/Utility.h"
#include "d3d/Camera.h"
#include "model.h"
#include "depthShader.h"
#include "sceneShader.h"
#include "Device.h"

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

	void v_OnMouseDown(WPARAM btnState, int x, int y);
	void v_OnMouseMove(WPARAM btnState, int x, int y);
	void v_OnMouseUp(WPARAM btnState, int x, int y);
	void v_OnMouseWheel(WPARAM btnState, int x, int y);

private:

	d3d::Camera m_Camera;

	byhj::Device m_pDevice;
	byhj::Model  m_CubeModel;
	byhj::Model  m_SphereModel;
	byhj::Model  m_PlaneModel;
	byhj::DepthShader depthShader;
	byhj::SceneShader sceneShader;
						    
	ComPtr<ID3D11ShaderResourceView> m_pWallTex;
	ComPtr<ID3D11ShaderResourceView> m_pIceTex;
	ComPtr<ID3D11ShaderResourceView> m_pMetalTex;

	d3d::MatrixBuffer m_Matrix;
};
}
#endif