#ifndef D3DUTILITY_H
#define D3DUTILITY_H

#include <windows.h>


#include <dxerr.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <dxgi.h>
#include <d3d11.h>
#include <d3dcommon.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

#pragma  comment(lib, "dxerr")
#pragma  comment(lib, "d3d11")
#pragma  comment(lib, "dxgi")
#pragma  comment(lib, "dxerr")

namespace byhj
{

    struct D3DMVPMatrix {

			XMFLOAT4X4 model;
			XMFLOAT4X4 view;
			XMFLOAT4X4 proj;
	};
}

#define ARRAYSIZE(arr) sizeof(arr) / sizeof(arr[0])


//---------------------------------------------------------------------------------------
// Simple d3d error checker for book demos.
//---------------------------------------------------------------------------------------

#ifndef DebugHR()
#define DebugHR(hr)                                                  \
{                                                                \
	HRESULT result = hr;                                             \
if ( FAILED(result) )                                               \
{                                                                \
	DXTrace( (const WCHAR * )__FILE__, ( DWORD )__LINE__, result, L"Error", true);  \
}                                                                \
}
#endif

//---------------------------------------------------------------------------------------
// Convenience macro for releasing COM objects.
//---------------------------------------------------------------------------------------

#define ReleaseCOM(x) { if(x){ x->Release(); x = 0; } }

//---------------------------------------------------------------------------------------
// Convenience macro for deleting objects.
//---------------------------------------------------------------------------------------

#define SafeDelete(x) { delete x; x = 0; }

#endif