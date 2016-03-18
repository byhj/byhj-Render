#ifndef UTILITY_H
#define UTILITY_H

#include <windows.h>


#include <dxerr.h>
#include <DirectXMath.h>
#include <wrl.h>

using namespace DirectX;
using namespace Microsoft::WRL;

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