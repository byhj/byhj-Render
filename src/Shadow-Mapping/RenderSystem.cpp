#include "RenderSystem.h"
#include "DirectXTK/DDSTextureLoader.h"

namespace byhj
{

RenderSystem::RenderSystem() 
{
	m_AppName = L"DirectX11-Init D3D";
}

RenderSystem::~RenderSystem()
{

}

void RenderSystem::v_Init()
{
	m_pDevice.Init(m_ScreenWidth, m_ScreenHeight, GetHwnd());

	ID3D11Device *pD3D11Device = m_pDevice.GetDevice();
	ID3D11DeviceContext *pD3D11DeviceContext  = m_pDevice.GetDeviceContext();
	m_Camera.SetRadius(7.0f);

	depthShader.Init(pD3D11Device, pD3D11DeviceContext, GetHwnd());
	sceneShader.Init(pD3D11Device, pD3D11DeviceContext, GetHwnd());

	m_CubeModel.load_model("../../media/objects/cube.txt");
	m_CubeModel.init_buffer(pD3D11Device, pD3D11DeviceContext);

	m_SphereModel.load_model("../../media/objects/sphere.txt");
	m_SphereModel.init_buffer(pD3D11Device, pD3D11DeviceContext);

	m_PlaneModel.load_model("../../media/objects/plane01.txt");
	m_PlaneModel.init_buffer(pD3D11Device, pD3D11DeviceContext);

	CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/wall01.dds", NULL, &m_pWallTex);
	CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/ice.dds", NULL, &m_pIceTex);
	CreateDDSTextureFromFile(pD3D11Device, L"../../media/textures/metal001.dds", NULL, &m_pMetalTex);
}

void RenderSystem::v_Update()
{
	m_Camera.update();
}

void RenderSystem::v_Render()
{
	m_Matrix = m_pDevice.GetMatrix();

	m_pDevice.BeginRTT();

	XMMATRIX Model = XMMatrixIdentity();
	static float lightPositionX = -5.0f;

	//lightPositionX += m_pDevice.GetDeltaTime();
	if (lightPositionX > 5.0f)
	{
		lightPositionX = -5.0f;
	}

	XMFLOAT4X4 lightView;
	XMVECTOR LightPos = XMVectorSet(lightPositionX, 8.0f, -5.0f, 1.0f);
	XMMATRIX LightView = XMMatrixLookAtLH(LightPos, XMVectorSet(0.0, 0.0, 0.0, 1.0), XMVectorSet(0.0f, 1.0, 0.0, 1.0));
	XMStoreFloat4x4(&lightView, XMMatrixTranspose(LightView));

	m_Matrix.view = lightView;

	Model = XMMatrixTranslation(-2.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));

	ID3D11DeviceContext *pD3D11DeviceContext  = m_pDevice.GetDeviceContext();

	depthShader.Use(pD3D11DeviceContext , m_Matrix);
	m_CubeModel.Render(pD3D11DeviceContext );

	Model = XMMatrixTranslation(2.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));

	depthShader.Use(pD3D11DeviceContext , m_Matrix);
	m_SphereModel.Render(pD3D11DeviceContext);

	Model = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));

	depthShader.Use(pD3D11DeviceContext , m_Matrix);
	m_PlaneModel.Render(pD3D11DeviceContext );

	////////////////////////////////////////////////////////////////////////////////

	m_pDevice.BeginScene();

	XMFLOAT4 lightPos;
	XMStoreFloat4(&lightPos, LightPos);

	m_Matrix.view = m_Camera.GetViewMatrix();
	XMMATRIX Proj = XMMatrixPerspectiveFovLH(0.4f*3.14f, GetAspect(), 1.0f, 1000.0f);
	XMStoreFloat4x4(&m_Matrix.proj, XMMatrixTranspose(Proj));


	pD3D11DeviceContext->PSSetShaderResources(1, 1, m_pDevice.GetSRV());
	pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pWallTex.GetAddressOf());

	Model = XMMatrixTranslation(-2.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
	sceneShader.Use(pD3D11DeviceContext, m_Matrix, lightView, m_Matrix.proj, lightPos);
	m_CubeModel.Render(pD3D11DeviceContext);

	pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pIceTex.GetAddressOf());
	Model = XMMatrixTranslation(2.0f, 2.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
	sceneShader.Use(pD3D11DeviceContext, m_Matrix, lightView, m_Matrix.proj, lightPos);
	m_SphereModel.Render(pD3D11DeviceContext);

	pD3D11DeviceContext->PSSetShaderResources(0, 1, m_pMetalTex.GetAddressOf());
	Model = XMMatrixTranslation(0.0f, 1.0f, 0.0f);
	XMStoreFloat4x4(&m_Matrix.model, XMMatrixTranspose(Model));
	sceneShader.Use(pD3D11DeviceContext, m_Matrix, lightView, m_Matrix.proj, lightPos);
	m_PlaneModel.Render(pD3D11DeviceContext);

	m_pDevice.DrawInfo();
	m_pDevice.EndScene();
}

void RenderSystem::v_Shutdown()
{

}

void  RenderSystem::v_OnMouseDown(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseDown(btnState, x, y, GetHwnd());
}

void  RenderSystem::v_OnMouseMove(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseMove(btnState, x, y);
}

void  RenderSystem::v_OnMouseUp(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseUp(btnState, x, y);
}
void  RenderSystem::v_OnMouseWheel(WPARAM btnState, int x, int y)
{
	m_Camera.OnMouseWheel(btnState, x, y, GetAspect());
}


}