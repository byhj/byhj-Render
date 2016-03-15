#ifndef DXUTHELPER
#define DXUTHELPER

#define DXUT_AUTOLIB

#include <memory.h>

#include "DXUT.h"
#include "SDKmisc.h"
#include "DXUTcamera.h"
#include "DXUTgui.h"
#include "DXUTsettingsdlg.h"
#include "RenderSystem.h"

#include "d3d/d3dUtility.h"

#pragma comment(lib, "DXUT.lib")
#pragma comment(lib, "DXUTOpt.lib")

namespace byhj
{

class DXUTHelper
{
public:
	DXUTHelper();
	~DXUTHelper();

public:

	void init();
	void render();
	void shutdown();

private:
	void render_text();
	void init_gui();

#pragma region CALLFUNC


	static void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);
	static void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	static void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
	static void CALLBACK OnMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
		bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
		int xPos, int yPos, void* pUserContext);

	static LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext);

	static bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
	static bool CALLBACK OnDeviceRemoved(void* pUserContext);

	////////////////////////////////////////////////////////////////////////////////////////////////////
	static bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
		DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);

	static HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext);

	static HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

	static void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
		double fTime, float fElapsedTime, void* pUserContext);

	static void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext);
	static void CALLBACK OnD3D11DestroyDevice(void* pUserContext);


	///////////////////////////////////////////////////////////////////////////////////////////////
	void CALLBACK AppGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext);


	void CALLBACK AppFrameMove(double fTime, float fElapsedTime, void* pUserContext);
	void CALLBACK AppKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext);
	void CALLBACK AppMouse(bool bLeftButtonDown, bool bRightButtonDown, bool bMiddleButtonDown,
		bool bSideButton1Down, bool bSideButton2Down, int nMouseWheelDelta,
		int xPos, int yPos, void* pUserContext);

	LRESULT CALLBACK AppMsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		bool* pbNoFurtherProcessing, void* pUserContext);

	bool CALLBACK AppModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext);
	bool CALLBACK AppDeviceRemoved(void* pUserContext);

	bool CALLBACK AppD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
	                                    	DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext);

	HRESULT CALLBACK AppD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
	                                  	 void* pUserContext);

	HRESULT CALLBACK AppD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		                                    const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext);

	void CALLBACK AppD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext,
		                             double fTime, float fElapsedTime, void* pUserContext);

	void CALLBACK AppD3D11ReleasingSwapChain(void* pUserContext);
	void CALLBACK AppD3D11DestroyDevice(void* pUserContext);


#pragma endregion

	CModelViewerCamera         m_Camera;
	CDXUTDialogResourceManager m_DialogResourceManager;
	CD3DSettingsDlg            m_SettingsDlg;
	CDXUTDialog                m_HUD;
	CDXUTDialog                m_SampleUI;
	CDXUTTextHelper           *m_pTextHelper = nullptr;

	float                      m_fModelPuffiness = 0.0f;
	bool                       m_bSpinning = true;
	int m_ScreenWidth = 0;
	int m_ScreenHeight = 0;

	byhj::RenderSystem m_RenderSystem;
	static std::shared_ptr<DXUTHelper> pDXUTHelper;

	D3DMVPMatrix m_Matrix;

};

}

#endif