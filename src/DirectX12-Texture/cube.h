#include "d3d/d3d12Util.h"
#include "d3d/d3dMathHelper.h"
#include "UploadBuffer.h"
#include "FrameResource.h"

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

	void UpdateObjectCBs();
	void UpdateMaterialCBs();
	void UpdateMainPassCB();

	void LoadTextures();
	void BuildDescriptorHeaps();
	void BuildConstantBuffers();
	void BuildRootSignature();
	void BuildShadersAndInputLayout();
	void BuildBoxGeometry();
	void BuildPSO();
	void BuildFrameResources();
	void BuildMaterials();
	void BuildRenderItems();
	
	struct RenderItem
	{
		RenderItem() = default;

		// World matrix of the shape that describes the object's local space
		// relative to the world space, which defines the position, orientation,
		// and scale of the object in the world.
		XMFLOAT4X4 World = D3DMathHelper::Identity4x4();

		XMFLOAT4X4 TexTransform = D3DMathHelper::Identity4x4();

		// Dirty flag indicating the object data has changed and we need to update the constant buffer.
		// Because we have an object cbuffer for each FrameResource, we have to apply the
		// update to each FrameResource.  Thus, when we modify obect data we should set 
		// NumFramesDirty = gNumFrameResources so that each frame resource gets the update.
		int NumFramesDirty = gNumFrameResources;

		// Index into GPU constant buffer corresponding to the ObjectCB for this render item.
		UINT ObjCBIndex = -1;

		Material* Mat = nullptr;
		MeshGeometry* Geo = nullptr;

		// Primitive topology.
		D3D12_PRIMITIVE_TOPOLOGY PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		// DrawIndexedInstanced parameters.
		UINT IndexCount = 0;
		UINT StartIndexLocation = 0;
		int BaseVertexLocation = 0;
	};
	
	
	void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
	
	std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> GetStaticSamplers();

	struct Vertex
	{
		Vertex(XMFLOAT3 pos, XMFLOAT3 normal, XMFLOAT2 tex) : Pos(pos), Normal(normal), Tex(tex) {}
		XMFLOAT3 Pos;
		XMFLOAT3 Normal;
		XMFLOAT2 Tex;
	};

	struct ObjectConstants
	{
		XMFLOAT4X4 WorldViewProj = D3DMathHelper::Identity4x4();
	};

		ComPtr<ID3D12PipelineState> getPSO() {
		   return mPSO;
		}
private:
	const int mNumFrameResources = 3;
	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;
	// Render items divided by PSO.
	std::vector<RenderItem*> mOpaqueRitems;
	std::vector<std::unique_ptr<FrameResource>> mFrameResources;
	FrameResource* mCurrFrameResource = nullptr;
	PassConstants mMainPassCB;

	ComPtr<ID3D12RootSignature> mRootSignature = nullptr;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap = nullptr;
	 UINT mCbvSrvDescriptorSize = 0;

	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;

	std::unique_ptr<MeshGeometry> mBoxGeo = nullptr;

	ComPtr<ID3DBlob> mvsByteCode = nullptr;
	ComPtr<ID3DBlob> mpsByteCode = nullptr;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	ComPtr<ID3D12PipelineState> mPSO = nullptr;
	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

	XMFLOAT4X4 mWorld = D3DMathHelper::Identity4x4();
	XMFLOAT4X4 mView  = D3DMathHelper::Identity4x4();
	XMFLOAT4X4 mProj  = D3DMathHelper::Identity4x4();


	ComPtr<ID3D12Device> md3dDevice;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12CommandAllocator> mDirectCmdListAlloc;
};


}