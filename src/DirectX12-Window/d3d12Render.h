#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3d/d3dApp.h"
#include "d3d/d3d12Util.h"
#include "timer.h"

#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

namespace byhj {

class D3D12Render : public D3DApp
{
public:
	D3D12Render();
	~D3D12Render();

	void v_init()     override;
	void v_update()   override;
	void v_render()   override;
	void v_shutdown() override;

    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);

protected:

	void init_d3d();
	void init_swapchain();
	void init_cmd();
	void init_descHeaps();
	void init_size();
	void calc_fps();
	void flush_cmdQueue();

	ID3D12Resource* CurrentBackBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView() const;

    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:
	// Set true to use 4X MSAA (?.1.8).  The default is false.
    bool      m_4xMsaaState = false;    // 4X MSAA enabled
    UINT      m_4xMsaaQuality = 0;      // quality level of 4X MSAA

	// Used to keep track of the “delta-time?and game time (?.4).
	Timer  m_Timer;
	UINT64 m_CurrentFence = 0;
	int    m_CurrBackBuffer = 0;
	static const int m_SwapChainBufferCount = 2;


	// D3D12 COM Objects
    ComPtr<IDXGIFactory4>             m_pFactory;
    ComPtr<IDXGISwapChain>            m_pSwapChain;
    ComPtr<ID3D12Device>              m_pD3D12Device;
    ComPtr<ID3D12Fence>               m_pFence;

    ComPtr<ID3D12CommandQueue>        m_pCommandQueue;
    ComPtr<ID3D12CommandAllocator>    m_pCommandAlloc;
    ComPtr<ID3D12GraphicsCommandList> m_pCommandList;

    ComPtr<ID3D12Resource>            m_pSwapChainBuffer[m_SwapChainBufferCount];
    ComPtr<ID3D12Resource>            m_pDepthStencilBuffer;
    ComPtr<ID3D12DescriptorHeap>      m_pRendetTargetViewHeap;
    ComPtr<ID3D12DescriptorHeap>      m_pDepthStencilViewHeap;

    D3D12_VIEWPORT m_ScreenViewport; 
    D3D12_RECT m_ScissorRect;

	UINT m_RtvDescriptorSize       = 0;
	UINT m_DsvDescriptorSize       = 0;
	UINT m_CbvSrvUavDescriptorSize = 0;

	D3D_DRIVER_TYPE m_D3DDriverType      = D3D_DRIVER_TYPE_HARDWARE;
	DXGI_FORMAT     m_BackBufferFormat   = DXGI_FORMAT_R8G8B8A8_UNORM;
	DXGI_FORMAT     m_DepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	int mClientWidth = 800;
	int mClientHeight = 600;
};

}