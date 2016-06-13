#include "d3d/d3d12Util.h"
#include "d3d/d3dMathHelper.h"
#include "UploadBuffer.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

namespace byhj {


class Cube
{
public:
	void init(ComPtr<ID3D12Device> md3dDevice, ComPtr<ID3D12GraphicsCommandList> mCommandList, 
		      ComPtr<ID3D12CommandQueue> mCommandQueue);
	void update(float deltaTime);
	void render();
	void shutdown();

	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildBoxGeometry();
	void BuildPSO();

	struct Vertex
	{
		Vertex(XMFLOAT3 pos, XMFLOAT4 color) : Pos(pos), Color(color) {}
		XMFLOAT3 Pos;
		XMFLOAT4 Color;
	};

	struct ObjectConstants
	{
		XMFLOAT4X4 WorldViewProj = D3DMathHelper::Identity4x4();
	};

		ComPtr<ID3D12PipelineState> getPSO() {
		   return mPSO;
		}
private:


	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;

	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mPSO = nullptr;

	XMFLOAT4X4 mWorld = D3DMathHelper::Identity4x4();
	XMFLOAT4X4 mView  = D3DMathHelper::Identity4x4();
	XMFLOAT4X4 mProj  = D3DMathHelper::Identity4x4();


	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
};


}