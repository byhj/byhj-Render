#include "d3dUtility.h"

IDirect3DDevice9 *Device = 0;
const int Width = 1000;
const int Height = 800;

IDirect3DVertexBuffer9 *VBO = 0;
IDirect3DIndexBuffer9 *IBO = 0;

struct Vertex
{
	Vertex(){}
	Vertex( float x, float y, float z)
	{
		_x = x; _y = y; _z = z;  
	}
	float _x, _y, _z;
	static const DWORD FVF; //静态成员在外面定义
};

const DWORD Vertex::FVF = D3DFVF_XYZ;

bool Setup()
{
	//Create the Vertex Buffer
	Device->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY, // usage
		Vertex::FVF,        // Vertex format
		D3DPOOL_MANAGED,    // managed memory pool
		&VBO,          // return create vertex buffer
		0);

   //create the index buffer
    Device->CreateIndexBuffer(
		 36 * sizeof(WORD),
		 D3DUSAGE_WRITEONLY,
		 D3DFMT_INDEX16,
		 D3DPOOL_MANAGED,
		 &IBO,
		 0);

	//fill the vertex buffer
	Vertex *VertexData;
	VBO->Lock(0, 0, (void**)&VertexData, 0);

	VertexData[0] = Vertex(-1.0f, -1.0f, -1.0f);
	VertexData[1] = Vertex(-1.0f,  1.0f, -1.0f);
	VertexData[2] = Vertex( 1.0f,  1.0f, -1.0f);
	VertexData[3] = Vertex( 1.0f, -1.0f, -1.0f);
	VertexData[4] = Vertex(-1.0f, -1.0f,  1.0f);
	VertexData[5] = Vertex(-1.0f,  1.0f,  1.0f);
	VertexData[6] = Vertex( 1.0f,  1.0f,  1.0f);
	VertexData[7] = Vertex( 1.0f, -1.0f,  1.0f);

	VBO->Unlock();

	WORD *ElementData = 0;
	IBO->Lock(0, 0, (void**)&ElementData, 0);
	// front side
	ElementData[0]  = 0; ElementData[1]  = 1; ElementData[2]  = 2;
	ElementData[3]  = 0; ElementData[4]  = 2; ElementData[5]  = 3;

	// back side
	ElementData[6]  = 4; ElementData[7]  = 6; ElementData[8]  = 5;
	ElementData[9]  = 4; ElementData[10] = 7; ElementData[11] = 6;

	// left side
	ElementData[12] = 4; ElementData[13] = 5; ElementData[14] = 1;
	ElementData[15] = 4; ElementData[16] = 1; ElementData[17] = 0;

	// right side
	ElementData[18] = 3; ElementData[19] = 2; ElementData[20] = 6;
	ElementData[21] = 3; ElementData[22] = 6; ElementData[23] = 7;

	// top
	ElementData[24] = 1; ElementData[25] = 5; ElementData[26] = 6;
	ElementData[27] = 1; ElementData[28] = 6; ElementData[29] = 2;

	// bottom
	ElementData[30] = 4; ElementData[31] = 0; ElementData[32] = 3;
	ElementData[33] = 4; ElementData[34] = 3; ElementData[35] = 7;

	IBO->Unlock();

	// set the view_matrix
	D3DXVECTOR3 pos(0.0f, 0.0f, -5.0f);
	D3DXVECTOR3 targer(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMATRIX view;
	D3DXMatrixLookAtLH(&view, &pos, &targer, &up);
	Device->SetTransform(D3DTS_VIEW, &view);

	//set the projection matrix
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f,
		(float)Width / (float)Height,
		1.0f,
		1000.0f);
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	//set wireframe mode render status
	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	
	return true;
}

void CleanUp()
{
	byhj::Release<IDirect3DVertexBuffer9*>(VBO);
	byhj::Release<IDirect3DIndexBuffer9*>(IBO);
}

bool render(float timeDelta)
{
	if (Device)
	{
		D3DXMATRIX rx, ry;
		D3DXMatrixRotationX(&rx, 3.14f / 4.0f);
		// incremement y-rotation angle each frame
		static float y = 0.0f;
		D3DXMatrixRotationY(&ry, y);
		y += timeDelta;
		// reset angle to zero when angle reaches 2*PI
		if( y >= 6.28f )
			y = 0.0f;

		D3DXMATRIX model = rx * ry;
		Device->SetTransform(D3DTS_WORLD, &model);

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();

		Device->SetStreamSource(0, VBO, 0, sizeof(Vertex));
		Device->SetIndices(IBO);
		Device->SetFVF(Vertex::FVF);
		Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}


//
// WndProc
//
LRESULT CALLBACK byhj::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			::DestroyWindow(hwnd);
		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
				   HINSTANCE prevInstance, 
				   PSTR cmdLine,
				   int showCmd)
{
	if(!byhj::init(hinstance, Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, "InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if(!Setup())
	{
		::MessageBox(0, "Setup() - FAILED", 0, 0);
		return 0;
	}

	byhj::EnterMsgLoop(render);

	CleanUp();

	Device->Release();

	return 0;
}
