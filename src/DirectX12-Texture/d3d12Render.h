#pragma once

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

#include "d3d12Util.h"
#include "d3d/d3dApp.h"
#include "timer.h"

// Link necessary d3d12 libraries.
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

using Microsoft::WRL::ComPtr;
using namespace std;
using namespace DirectX;

namespace byhj {

class D3D12Render : public D3DApp
{

public:

	D3D12Render();
	~D3D12Render();

	void v_init()      override;
	void v_update()    override;
	void v_render()    override;
	void v_shutdown()  override;

   
    bool Get4xMsaaState()const;
    void Set4xMsaaState(bool value);
	void OnResize();

protected:
	bool InitDirect3D();
	void CreateCommandObjects();
    void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void FlushCommandQueue();

	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;


    void LogAdapters();
    void LogAdapterOutputs(IDXGIAdapter* adapter);
    void LogOutputDisplayModes(IDXGIOutput* output, DXGI_FORMAT format);

protected:


    HINSTANCE mhAppInst = nullptr; // application instance handle
    HWND      mhMainWnd = nullptr; // main window handle
	bool      mAppPaused = false;  // is the application paused?
	bool      mMinimized = false;  // is the application minimized?
	bool      mMaximized = false;  // is the application maximized?
	bool      mResizing = false;   // are the resize bars being dragged?
    bool      mFullscreenState = false;// fullscreen enabled

	// Set true to use 4X MSAA (?.1.8).  The default is false.
    bool      m4xMsaaState = false;    // 4X MSAA enabled
    UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA


    UINT64 mCurrentFence = 0;
	static const int SwapChainBufferCount = 2;
	int mCurrBackBuffer = 0;

    D3D12_VIEWPORT mScreenViewport; 
    D3D12_RECT mScissorRect;
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;

	// Derived class should set these in derived constructor to customize starting values.

	D3D_DRIVER_TYPE md3dDriverType = D3D_DRIVER_TYPE_HARDWARE;
    DXGI_FORMAT mBackBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ComPtr<IDXGIFactory4> mdxgiFactory;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12Fence> mFence;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12Resource> mSwapChainBuffer[SwapChainBufferCount];
	ComPtr<ID3D12Resource> mDepthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;

};

}