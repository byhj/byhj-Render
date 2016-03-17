#ifndef D3DPICKING_H
#define D3DPICKING_H

#include <windows.h>
#include <vector>
#include <DirectXMath.h> 
#include <DirectXCollision.h>

using namespace DirectX;

class D3DPicking
{
public:
	D3DPicking() 
	{
		m_pickRayPos = XMVECTOR();
		m_pickRayDir = XMVECTOR();
		m_ClientWidth  = 0;
		m_ClientHeight = 0;
	}
	~D3DPicking() {}

public:
	void InitPicking(int sw, int sh, std::vector<XMFLOAT3> vPos, std::vector<UINT> vIndex)
	{
		m_ClientWidth = sw;
		m_ClientHeight = sh;
		m_PosData = vPos;
		m_IndexData = vIndex;
	}

	void PickRayVector(float mouseX, float mouseY, XMFLOAT3 camPos,XMFLOAT4X4 camView, XMFLOAT4X4 camProj);

	float Pick(const XMMATRIX& worldSpace);

	bool PointInTriangle(XMVECTOR& triV1, XMVECTOR& triV2, XMVECTOR& triV3, XMVECTOR& point );

private:

	XMVECTOR m_pickRayPos;
	XMVECTOR m_pickRayDir;
	int m_ClientWidth;
	int m_ClientHeight;

	std::vector<XMFLOAT3> m_PosData;
	std::vector<UINT> m_IndexData;
	
};	
#endif