#ifndef Device_H
#define Device_H

#include <d3d11.h>
#include <string>

#include "d3d/Utility.h"
#include "d3d/Font.h"
#include "d3d/Timer.h"
#include "d3d/Camera.h"



namespace byhj
{
	class Device
	{

	public:
		Device() = default;
		~Device() = default;

		void Init(float sw, float sh, HWND hWnd);
		void Update();
		void Render();
		void Shutdown();

		void BeginScene();
		void EndScene();

		void BeginRTT();
		void EndRTT();
		d3d::MatrixBuffer GetMatrix()
		{
			return m_Matrix;
		}
		float GetDeltaTime();
		void EnableZBuffer();
		void DisableZBuffer();
		void DrawFps();
		void DrawInfo();

		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetDeviceContext();
		ID3D11ShaderResourceView ** GetSRV();
	private:

		void init_device(HWND hWnd);
		void init_fbo();
		void init_camera();

		d3d::Font m_Font;
		d3d::Timer m_Timer;

		ComPtr<IDXGISwapChain         > m_pSwapChain;
		ComPtr<ID3D11Device           > m_pD3D11Device;
		ComPtr<ID3D11DeviceContext    > m_pD3D11DeviceContext;
		ComPtr<ID3D11RenderTargetView > m_pRenderTargetView;
		ComPtr<ID3D11DepthStencilView > m_pDepthStencilView;
		ComPtr<ID3D11Texture2D        > m_pDepthStencilBuffer;
		ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;
		ComPtr<ID3D11DepthStencilState> m_pDepthDisabledStencilState;
		ComPtr<ID3D11RasterizerState  > m_pRasterState;

		ComPtr<ID3D11Texture2D> m_pRttRenderTargetTexture;
		ComPtr<ID3D11RenderTargetView> m_pRttRenderTargetView;
		ComPtr<ID3D11ShaderResourceView> m_pRttShaderResourceView;

		float fps = 0.0f;
		int m_videoCardMemory;
		std::wstring m_videoCardInfo;
		d3d::MatrixBuffer m_Matrix;
		float m_ScreenWidth = 0.0f;
		float m_ScreenHeight = 0.0f;
	};
}
#endif