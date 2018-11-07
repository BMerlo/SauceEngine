/*##############################################################################
#																			   #
# Copyright 2018 Sauce Pig All Rights Reserved.					               #
# Developed by Boris Merlo, Michael Noseworthy and Peter Tu                    #
#                                                                              #
##############################################################################*/


#include "DirectX/d3dApp.h"
#include "DirectX/MathHelper.h"
#include "DirectX/UploadBuffer.h"
#include "DirectX/GeometryGenerator.h"
#include "FrameResource.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")

const int gNumFrameResources = 3;

// Lightweight structure stores parameters to draw a shape.  This will
// vary from app-to-app.
struct RenderItem
{
	RenderItem() = default;

    // World matrix of the shape that describes the object's local space
    // relative to the world space, which defines the position, orientation,
    // and scale of the object in the world.
    XMFLOAT4X4 World = MathHelper::Identity4x4();

	XMFLOAT4X4 TexTransform = MathHelper::Identity4x4();

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

class LitColumnsApp : public D3DApp
{
public:
    LitColumnsApp(HINSTANCE hInstance);
    LitColumnsApp(const LitColumnsApp& rhs) = delete;
    LitColumnsApp& operator=(const LitColumnsApp& rhs) = delete;
    ~LitColumnsApp();

    virtual bool Initialize()override;

private:
    virtual void OnResize()override;
    virtual void Update(const GameTimer& gt)override;
    virtual void Draw(const GameTimer& gt)override;

    virtual void OnMouseDown(WPARAM btnState, int x, int y)override;
    virtual void OnMouseUp(WPARAM btnState, int x, int y)override;
    virtual void OnMouseMove(WPARAM btnState, int x, int y)override;

    void OnKeyboardInput(const GameTimer& gt);
	void UpdateCamera(const GameTimer& gt);
	void AnimateMaterials(const GameTimer& gt);
	void UpdateObjectCBs(const GameTimer& gt);
	void UpdateMaterialCBs(const GameTimer& gt);
	void UpdateMainPassCB(const GameTimer& gt);

    void BuildRootSignature();
    void BuildShadersAndInputLayout();
    void BuildShapeGeometry();
	void BuildSkullGeometry();
    void BuildPSOs();
    void BuildFrameResources();
    void BuildMaterials();
    void BuildRenderItems();
    void DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems);
 
private:

    std::vector<std::unique_ptr<FrameResource>> mFrameResources;
    FrameResource* mCurrFrameResource = nullptr;
    int mCurrFrameResourceIndex = 0;

    UINT mCbvSrvDescriptorSize = 0;

    ComPtr<ID3D12RootSignature> mRootSignature = nullptr;

	ComPtr<ID3D12DescriptorHeap> mSrvDescriptorHeap = nullptr;

	std::unordered_map<std::string, std::unique_ptr<MeshGeometry>> mGeometries;
	std::unordered_map<std::string, std::unique_ptr<Material>> mMaterials;
	std::unordered_map<std::string, std::unique_ptr<Texture>> mTextures;
	std::unordered_map<std::string, ComPtr<ID3DBlob>> mShaders;

    std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

    ComPtr<ID3D12PipelineState> mOpaquePSO = nullptr;
 
	// List of all the render items.
	std::vector<std::unique_ptr<RenderItem>> mAllRitems;

	// Render items divided by PSO.
	std::vector<RenderItem*> mOpaqueRitems;

    PassConstants mMainPassCB;

	XMFLOAT3 mEyePos = { 0.0f, 15.0f, 0.0f };
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

    float mTheta = 1.5f*XM_PI;
    float mPhi = 0.2f*XM_PI;
    float mRadius = 15.0f;

    POINT mLastMousePos;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
    PSTR cmdLine, int showCmd)
{
    // Enable run-time memory check for debug builds.
#if defined(DEBUG) | defined(_DEBUG)
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    try
    {
        LitColumnsApp theApp(hInstance);
        if(!theApp.Initialize())
            return 0;

        return theApp.Run();
    }
    catch(DxException& e)
    {
        MessageBox(nullptr, e.ToString().c_str(), L"HR Failed", MB_OK);
        return 0;
    }
}

LitColumnsApp::LitColumnsApp(HINSTANCE hInstance)
    : D3DApp(hInstance)
{
}

LitColumnsApp::~LitColumnsApp()
{
    if(md3dDevice != nullptr)
        FlushCommandQueue();
}

bool LitColumnsApp::Initialize()
{
    if(!D3DApp::Initialize())
        return false;

    // Reset the command list to prep for initialization commands.
    ThrowIfFailed(mCommandList->Reset(mDirectCmdListAlloc.Get(), nullptr));

    // Get the increment size of a descriptor in this heap type.  This is hardware specific, 
	// so we have to query this information.
    mCbvSrvDescriptorSize = md3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    BuildRootSignature();
    BuildShadersAndInputLayout();
    BuildShapeGeometry();
	BuildSkullGeometry();
	BuildMaterials();
    BuildRenderItems();
    BuildFrameResources();
    BuildPSOs();

    // Execute the initialization commands.
    ThrowIfFailed(mCommandList->Close());
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Wait until initialization is complete.
    FlushCommandQueue();

    return true;
}
 
void LitColumnsApp::OnResize()
{
    D3DApp::OnResize();

    // The window resized, so update the aspect ratio and recompute the projection matrix.
    XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*MathHelper::Pi, AspectRatio(), 1.0f, 1000.0f);
    XMStoreFloat4x4(&mProj, P);
}

void LitColumnsApp::Update(const GameTimer& gt)
{
    OnKeyboardInput(gt);
	UpdateCamera(gt);

    // Cycle through the circular frame resource array.
    mCurrFrameResourceIndex = (mCurrFrameResourceIndex + 1) % gNumFrameResources;
    mCurrFrameResource = mFrameResources[mCurrFrameResourceIndex].get();

    // Has the GPU finished processing the commands of the current frame resource?
    // If not, wait until the GPU has completed commands up to this fence point.
    if(mCurrFrameResource->Fence != 0 && mFence->GetCompletedValue() < mCurrFrameResource->Fence)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
        ThrowIfFailed(mFence->SetEventOnCompletion(mCurrFrameResource->Fence, eventHandle));
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }

	AnimateMaterials(gt);
	UpdateObjectCBs(gt);
	UpdateMaterialCBs(gt);
	UpdateMainPassCB(gt);
}

void LitColumnsApp::Draw(const GameTimer& gt)
{
    auto cmdListAlloc = mCurrFrameResource->CmdListAlloc;

    // Reuse the memory associated with command recording.
    // We can only reset when the associated command lists have finished execution on the GPU.
    ThrowIfFailed(cmdListAlloc->Reset());

    // A command list can be reset after it has been added to the command queue via ExecuteCommandList.
    // Reusing the command list reuses memory.
    ThrowIfFailed(mCommandList->Reset(cmdListAlloc.Get(), mOpaquePSO.Get()));

    mCommandList->RSSetViewports(1, &mScreenViewport);
    mCommandList->RSSetScissorRects(1, &mScissorRect);

    // Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

    // Clear the back buffer and depth buffer.
    mCommandList->ClearRenderTargetView(CurrentBackBufferView(), Colors::Orange, 0, nullptr);
    mCommandList->ClearDepthStencilView(DepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    // Specify the buffers we are going to render to.
    mCommandList->OMSetRenderTargets(1, &CurrentBackBufferView(), true, &DepthStencilView());

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	auto passCB = mCurrFrameResource->PassCB->Resource();
	mCommandList->SetGraphicsRootConstantBufferView(2, passCB->GetGPUVirtualAddress());

    DrawRenderItems(mCommandList.Get(), mOpaqueRitems);

    // Indicate a state transition on the resource usage.
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(CurrentBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

    // Done recording commands.
    ThrowIfFailed(mCommandList->Close());

    // Add the command list to the queue for execution.
    ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
    mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

    // Swap the back and front buffers
    ThrowIfFailed(mSwapChain->Present(0, 0));
	mCurrBackBuffer = (mCurrBackBuffer + 1) % SwapChainBufferCount;

    // Advance the fence value to mark commands up to this fence point.
    mCurrFrameResource->Fence = ++mCurrentFence;

    // Add an instruction to the command queue to set a new fence point. 
    // Because we are on the GPU timeline, the new fence point won't be 
    // set until the GPU finishes processing all the commands prior to this Signal().
    mCommandQueue->Signal(mFence.Get(), mCurrentFence);
}

void LitColumnsApp::OnMouseDown(WPARAM btnState, int x, int y)
{
    mLastMousePos.x = x;
    mLastMousePos.y = y;

    SetCapture(mhMainWnd);
}

void LitColumnsApp::OnMouseUp(WPARAM btnState, int x, int y)
{
    ReleaseCapture();
}

void LitColumnsApp::OnMouseMove(WPARAM btnState, int x, int y)
{
    if((btnState & MK_LBUTTON) != 0)
    {
        // Make each pixel correspond to a quarter of a degree.
        float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
        float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

        // Update angles based on input to orbit camera around box.
        mTheta += dx;
        mPhi += dy;

        // Restrict the angle mPhi.
        mPhi = MathHelper::Clamp(mPhi, 0.1f, MathHelper::Pi - 0.1f);
    }
    else if((btnState & MK_RBUTTON) != 0)
    {
        // Make each pixel correspond to 0.2 unit in the scene.
        float dx = 0.05f*static_cast<float>(x - mLastMousePos.x);
        float dy = 0.05f*static_cast<float>(y - mLastMousePos.y);

        // Update the camera radius based on input.
        mRadius += dx - dy;

        // Restrict the radius.
        mRadius = MathHelper::Clamp(mRadius, 5.0f, 150.0f);
    }

    mLastMousePos.x = x;
    mLastMousePos.y = y;
}
 
void LitColumnsApp::OnKeyboardInput(const GameTimer& gt)
{
}
 
void LitColumnsApp::UpdateCamera(const GameTimer& gt)
{
	// Convert Spherical to Cartesian coordinates.
	mEyePos.x = mRadius*sinf(mPhi)*cosf(mTheta);
	mEyePos.z = mRadius*sinf(mPhi)*sinf(mTheta);
	mEyePos.y = mRadius*cosf(mPhi);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(mEyePos.x, mEyePos.y, mEyePos.z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);
}

void LitColumnsApp::AnimateMaterials(const GameTimer& gt)
{
	
}

void LitColumnsApp::UpdateObjectCBs(const GameTimer& gt)
{
	auto currObjectCB = mCurrFrameResource->ObjectCB.get();
	for(auto& e : mAllRitems)
	{
		// Only update the cbuffer data if the constants have changed.  
		// This needs to be tracked per frame resource.
		if(e->NumFramesDirty > 0)
		{
			XMMATRIX world = XMLoadFloat4x4(&e->World);
			XMMATRIX texTransform = XMLoadFloat4x4(&e->TexTransform);

			ObjectConstants objConstants;
			XMStoreFloat4x4(&objConstants.World, XMMatrixTranspose(world));
			XMStoreFloat4x4(&objConstants.TexTransform, XMMatrixTranspose(texTransform));

			currObjectCB->CopyData(e->ObjCBIndex, objConstants);

			// Next FrameResource need to be updated too.
			e->NumFramesDirty--;
		}
	}
}

void LitColumnsApp::UpdateMaterialCBs(const GameTimer& gt)
{
	auto currMaterialCB = mCurrFrameResource->MaterialCB.get();
	for(auto& e : mMaterials)
	{
		// Only update the cbuffer data if the constants have changed.  If the cbuffer
		// data changes, it needs to be updated for each FrameResource.
		Material* mat = e.second.get();
		if(mat->NumFramesDirty > 0)
		{
			XMMATRIX matTransform = XMLoadFloat4x4(&mat->MatTransform);

			MaterialConstants matConstants;
			matConstants.DiffuseAlbedo = mat->DiffuseAlbedo;
			matConstants.FresnelR0 = mat->FresnelR0;
			matConstants.Roughness = mat->Roughness;
			XMStoreFloat4x4(&matConstants.MatTransform, XMMatrixTranspose(matTransform));

			currMaterialCB->CopyData(mat->MatCBIndex, matConstants);

			// Next FrameResource need to be updated too.
			mat->NumFramesDirty--;
		}
	}
}

void LitColumnsApp::UpdateMainPassCB(const GameTimer& gt)
{
	XMMATRIX view = XMLoadFloat4x4(&mView);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);

	XMMATRIX viewProj = XMMatrixMultiply(view, proj);
	XMMATRIX invView = XMMatrixInverse(&XMMatrixDeterminant(view), view);
	XMMATRIX invProj = XMMatrixInverse(&XMMatrixDeterminant(proj), proj);
	XMMATRIX invViewProj = XMMatrixInverse(&XMMatrixDeterminant(viewProj), viewProj);

	XMStoreFloat4x4(&mMainPassCB.View, XMMatrixTranspose(view));
	XMStoreFloat4x4(&mMainPassCB.InvView, XMMatrixTranspose(invView));
	XMStoreFloat4x4(&mMainPassCB.Proj, XMMatrixTranspose(proj));
	XMStoreFloat4x4(&mMainPassCB.InvProj, XMMatrixTranspose(invProj));
	XMStoreFloat4x4(&mMainPassCB.ViewProj, XMMatrixTranspose(viewProj));
	XMStoreFloat4x4(&mMainPassCB.InvViewProj, XMMatrixTranspose(invViewProj));
	mMainPassCB.EyePosW = mEyePos;
	mMainPassCB.RenderTargetSize = XMFLOAT2((float)mClientWidth, (float)mClientHeight);
	mMainPassCB.InvRenderTargetSize = XMFLOAT2(1.0f / mClientWidth, 1.0f / mClientHeight);
	mMainPassCB.NearZ = 1.0f;
	mMainPassCB.FarZ = 1000.0f;
	mMainPassCB.TotalTime = gt.TotalTime();
	mMainPassCB.DeltaTime = gt.DeltaTime();
	//mMainPassCB.AmbientLight = { 0.3f, 0.3f, 0.3f, 1.0f };
	mMainPassCB.AmbientLight = { 0.0f, 0.5f, 0.2f, 1.0f };
	
	//Middle
	mMainPassCB.Lights[0].Direction = { 0.0f, 0.0f, 0.0f };
	mMainPassCB.Lights[0].Strength = { 0.01f, 0.01f, 0.01f };
	mMainPassCB.Lights[1].Direction = { 0.0f, 4.0f, -15.0f };
	mMainPassCB.Lights[1].SpotPower = { 50.0f};
	mMainPassCB.Lights[1].Strength = { 0.1f, 0.1f, 0.1f };

	//left
	mMainPassCB.Lights[2].Direction = { 0.0f, 0.0f, 15.f };
	mMainPassCB.Lights[2].Strength = { 0.1f, 0.1f, 0.1f };
	mMainPassCB.Lights[3].Direction = { 0.0f, 4.0f, 15.0f };
	mMainPassCB.Lights[3].SpotPower = { 50.0f };
	mMainPassCB.Lights[3].Strength = { 0.1f, 0.1f, 0.1f };

	//Right
	mMainPassCB.Lights[4].Direction = { 0.0f, 0.0f, -15.f };
	mMainPassCB.Lights[4].Strength = { 0.1f, 0.1f, 0.1f };
	mMainPassCB.Lights[5].Direction = { 0.0f, 4.0f, -15.f };
	mMainPassCB.Lights[5].SpotPower = { 50.0f };
	mMainPassCB.Lights[5].Strength = { 0.1f, 0.1f, 0.1f };

	auto currPassCB = mCurrFrameResource->PassCB.get();
	currPassCB->CopyData(0, mMainPassCB);
}

void LitColumnsApp::BuildRootSignature()
{
	// Root parameter can be a table, root descriptor or root constants.
	CD3DX12_ROOT_PARAMETER slotRootParameter[3];

	// Create root CBV.
	slotRootParameter[0].InitAsConstantBufferView(0);
	slotRootParameter[1].InitAsConstantBufferView(1);
	slotRootParameter[2].InitAsConstantBufferView(2);

	// A root signature is an array of root parameters.
	CD3DX12_ROOT_SIGNATURE_DESC rootSigDesc(3, slotRootParameter, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	// create a root signature with a single slot which points to a descriptor range consisting of a single constant buffer
	ComPtr<ID3DBlob> serializedRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	HRESULT hr = D3D12SerializeRootSignature(&rootSigDesc, D3D_ROOT_SIGNATURE_VERSION_1,
		serializedRootSig.GetAddressOf(), errorBlob.GetAddressOf());

	if(errorBlob != nullptr)
	{
		::OutputDebugStringA((char*)errorBlob->GetBufferPointer());
	}
	ThrowIfFailed(hr);

	ThrowIfFailed(md3dDevice->CreateRootSignature(
		0,
		serializedRootSig->GetBufferPointer(),
		serializedRootSig->GetBufferSize(),
		IID_PPV_ARGS(mRootSignature.GetAddressOf())));
}

void LitColumnsApp::BuildShadersAndInputLayout()
{
	const D3D_SHADER_MACRO alphaTestDefines[] =
	{
		"ALPHA_TEST", "1",
		NULL, NULL
	};

	mShaders["standardVS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "VS", "vs_5_1");
	mShaders["opaquePS"] = d3dUtil::CompileShader(L"Shaders\\Default.hlsl", nullptr, "PS", "ps_5_1");
	
    mInputLayout =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };
}

void LitColumnsApp::BuildShapeGeometry()
{
    GeometryGenerator geoGen;
	GeometryGenerator::MeshData box = geoGen.CreateBox(1.5f, 0.5f, 1.5f, 3);
	GeometryGenerator::MeshData grid = geoGen.CreateGrid(100.0f, 100.0f, 60, 40);
	GeometryGenerator::MeshData sphere = geoGen.CreateSphere(0.5f, 20, 20);
	GeometryGenerator::MeshData cylinder = geoGen.CreateCylinder(0.5f, 0.3f, 3.0f, 20, 20);
	GeometryGenerator::MeshData diamond = geoGen.CreateDiamond(1, 1, 1, 3); /////////////////////////  ///1
	GeometryGenerator::MeshData pyramid = geoGen.CreatePyramid( 1 ); //pyramid
	GeometryGenerator::MeshData rhombo = geoGen.CreateRhombo(1); //rhombo
	GeometryGenerator::MeshData prism = geoGen.CreatePrism(1); //prism
	GeometryGenerator::MeshData hexagon = geoGen.CreateHexagon(4, 30, 4, 1); //hexagon
	GeometryGenerator::MeshData triangleEq = geoGen.CreateTriangleEq(1); //TriangleEq
	GeometryGenerator::MeshData triangleRectSqr = geoGen.CreateTriangleRectSqr(1); //triangleRectSqr
	GeometryGenerator::MeshData candy1 = geoGen.CreateCandy1(1, 1, 1, 1);
	GeometryGenerator::MeshData Box1 = geoGen.CreateBox1(5);
	GeometryGenerator::MeshData Box3 = geoGen.CreateBox3(1);
	GeometryGenerator::MeshData candy2 = geoGen.CreateCandy2(1);
	GeometryGenerator::MeshData candy3 = geoGen.CreateCandy3(1);

	


	//
	// We are concatenating all the geometry into one big vertex/index buffer.  So
	// define the regions in the buffer each submesh covers.
	//

	// Cache the vertex offsets to each object in the concatenated vertex buffer.
	UINT boxVertexOffset = 0;
	UINT gridVertexOffset = (UINT)box.Vertices.size();
	UINT sphereVertexOffset = gridVertexOffset + (UINT)grid.Vertices.size();
	UINT cylinderVertexOffset = sphereVertexOffset + (UINT)sphere.Vertices.size();
	UINT diamondVertexOffset = cylinderVertexOffset + (UINT)cylinder.Vertices.size();///////////////// //2
	UINT pyramidVertexOffset = diamondVertexOffset + (UINT)diamond.Vertices.size(); //pyramid
	UINT rhomboVertexOffset = pyramidVertexOffset + (UINT)pyramid.Vertices.size(); //rhombo
	UINT prismVertexOffset = rhomboVertexOffset + (UINT)rhombo.Vertices.size();
	UINT hexagonVertexOffset = prismVertexOffset + (UINT)prism.Vertices.size();
	UINT triangleEqVertexOffset = hexagonVertexOffset + (UINT)hexagon.Vertices.size();//TriangleEq
	UINT triangleRectSqrVertexOffset = triangleEqVertexOffset + (UINT)triangleEq.Vertices.size();	//triangleRectSqr
	UINT candy1VertexOffset = triangleRectSqrVertexOffset + (UINT)triangleRectSqr.Vertices.size();
	UINT Box1VertexOffset = candy1VertexOffset + (UINT)candy1.Vertices.size();
	UINT Box3VertexOffset = Box1VertexOffset + (UINT)Box1.Vertices.size();
	UINT candy2VertexOffset = Box3VertexOffset + (UINT)Box3.Vertices.size();
	UINT candy3VertexOffset = candy2VertexOffset + (UINT)candy2.Vertices.size();


	// Cache the starting index for each object in the concatenated index buffer.
	UINT boxIndexOffset = 0;
	UINT gridIndexOffset = (UINT)box.Indices32.size();
	UINT sphereIndexOffset = gridIndexOffset + (UINT)grid.Indices32.size();
	UINT cylinderIndexOffset = sphereIndexOffset + (UINT)sphere.Indices32.size();
	UINT diamondIndexOffset = cylinderIndexOffset + (UINT)cylinder.Indices32.size();/////////////////// //3
	UINT pyramidIndexOffset = diamondIndexOffset + (UINT)diamond.Indices32.size();//pyramind
	UINT rhomboIndexOffset = pyramidIndexOffset + (UINT)pyramid.Indices32.size();//rhombo
	UINT prismIndexOffset = rhomboIndexOffset + (UINT)rhombo.Indices32.size();//prism
	UINT hexagonIndexOffset = prismIndexOffset + (UINT)prism.Indices32.size();//hexagon
	UINT triangleEqIndexOffset = hexagonIndexOffset + (UINT)hexagon.Indices32.size();//TriangleEq
	UINT triangleRectSqrIndexOffset = triangleEqIndexOffset + (UINT)triangleEq.Indices32.size();//triangleRectSqr
	UINT candy1IndexOffset = triangleRectSqrIndexOffset + (UINT)triangleRectSqr.Indices32.size();
	UINT Box1IndexOffset = candy1IndexOffset + (UINT)candy1.Indices32.size();
	UINT Box3IndexOffset = Box1IndexOffset + (UINT)Box1.Indices32.size();
	UINT candy2IndexOffset = Box3IndexOffset + (UINT)Box3.Indices32.size();
	UINT candy3IndexOffset = candy2IndexOffset + (UINT)candy2.Indices32.size();

	SubmeshGeometry boxSubmesh;
	boxSubmesh.IndexCount = (UINT)box.Indices32.size();
	boxSubmesh.StartIndexLocation = boxIndexOffset;
	boxSubmesh.BaseVertexLocation = boxVertexOffset;

	SubmeshGeometry gridSubmesh;
	gridSubmesh.IndexCount = (UINT)grid.Indices32.size();
	gridSubmesh.StartIndexLocation = gridIndexOffset;
	gridSubmesh.BaseVertexLocation = gridVertexOffset;

	SubmeshGeometry sphereSubmesh;
	sphereSubmesh.IndexCount = (UINT)sphere.Indices32.size();
	sphereSubmesh.StartIndexLocation = sphereIndexOffset;
	sphereSubmesh.BaseVertexLocation = sphereVertexOffset;

	SubmeshGeometry cylinderSubmesh;
	cylinderSubmesh.IndexCount = (UINT)cylinder.Indices32.size();
	cylinderSubmesh.StartIndexLocation = cylinderIndexOffset;
	cylinderSubmesh.BaseVertexLocation = cylinderVertexOffset;

	SubmeshGeometry diamondSubmesh;///////////// //4
	diamondSubmesh.IndexCount = (UINT)diamond.Indices32.size();
	diamondSubmesh.StartIndexLocation = diamondIndexOffset;
	diamondSubmesh.BaseVertexLocation = diamondVertexOffset;

	SubmeshGeometry pyramidSubmesh;/////////////
	pyramidSubmesh.IndexCount = (UINT)pyramid.Indices32.size();
	pyramidSubmesh.StartIndexLocation = pyramidIndexOffset;
	pyramidSubmesh.BaseVertexLocation = pyramidVertexOffset;

	SubmeshGeometry rhomboSubmesh;///////////// rhombo
	rhomboSubmesh.IndexCount = (UINT)rhombo.Indices32.size();
	rhomboSubmesh.StartIndexLocation = rhomboIndexOffset;
	rhomboSubmesh.BaseVertexLocation = rhomboVertexOffset;

	SubmeshGeometry prismSubmesh;///////////// prism
	prismSubmesh.IndexCount = (UINT)prism.Indices32.size();
	prismSubmesh.StartIndexLocation = prismIndexOffset;
	prismSubmesh.BaseVertexLocation = prismVertexOffset;

	SubmeshGeometry hexagonSubmesh;///////////// hexagon
	hexagonSubmesh.IndexCount = (UINT)hexagon.Indices32.size();
	hexagonSubmesh.StartIndexLocation = hexagonIndexOffset;
	hexagonSubmesh.BaseVertexLocation = hexagonVertexOffset;

	SubmeshGeometry triangleEqSubmesh;///////////// TriangleEq
	triangleEqSubmesh.IndexCount = (UINT)triangleEq.Indices32.size();
	triangleEqSubmesh.StartIndexLocation = triangleEqIndexOffset;
	triangleEqSubmesh.BaseVertexLocation = triangleEqVertexOffset;

	SubmeshGeometry triangleRectSqrSubmesh;//triangleRectSqr
	triangleRectSqrSubmesh.IndexCount = (UINT)triangleRectSqr.Indices32.size();
	triangleRectSqrSubmesh.StartIndexLocation = triangleRectSqrIndexOffset;
	triangleRectSqrSubmesh.BaseVertexLocation = triangleRectSqrVertexOffset;

	SubmeshGeometry candy1Submesh;//triangleRectSqr
	candy1Submesh.IndexCount = (UINT)candy1.Indices32.size();
	candy1Submesh.StartIndexLocation = candy1IndexOffset;
	candy1Submesh.BaseVertexLocation = candy1VertexOffset;

	SubmeshGeometry Box1Submesh;//triangleRectSqr
	Box1Submesh.IndexCount = (UINT)Box1.Indices32.size();
	Box1Submesh.StartIndexLocation = Box1IndexOffset;
	Box1Submesh.BaseVertexLocation = Box1VertexOffset;

	SubmeshGeometry Box3Submesh;//triangleRectSqr
	Box3Submesh.IndexCount = (UINT)Box3.Indices32.size();
	Box3Submesh.StartIndexLocation = Box3IndexOffset;
	Box3Submesh.BaseVertexLocation = Box3VertexOffset;

	SubmeshGeometry candy2Submesh;//triangleRectSqr
	candy2Submesh.IndexCount = (UINT)candy2.Indices32.size();
	candy2Submesh.StartIndexLocation = candy2IndexOffset;
	candy2Submesh.BaseVertexLocation = candy2VertexOffset;

	SubmeshGeometry candy3Submesh;//triangleRectSqr
	candy3Submesh.IndexCount = (UINT)candy3.Indices32.size();
	candy3Submesh.StartIndexLocation = candy3IndexOffset;
	candy3Submesh.BaseVertexLocation = candy3VertexOffset;

	//
	// Extract the vertex elements we are interested in and pack the
	// vertices of all the meshes into one vertex buffer.
	//

	auto totalVertexCount =
		box.Vertices.size() +
		grid.Vertices.size() +
		sphere.Vertices.size() +
		cylinder.Vertices.size() +
		diamond.Vertices.size() +
		pyramid.Vertices.size() +///////////// //5
		rhombo.Vertices.size() +//rhombo
		prism.Vertices.size() + //prism
		hexagon.Vertices.size() +  //hexagon
		triangleEq.Vertices.size() + //TriangleEq
		triangleRectSqr.Vertices.size() +
		candy1.Vertices.size() +
		Box1.Vertices.size() +
		Box3.Vertices.size() +
		candy2.Vertices.size() +
		candy3.Vertices.size();

	std::vector<Vertex> vertices(totalVertexCount);

	UINT k = 0;
	for(size_t i = 0; i < box.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = box.Vertices[i].Position;
		vertices[k].Normal = box.Vertices[i].Normal;
	}

	for(size_t i = 0; i < grid.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = grid.Vertices[i].Position;
		vertices[k].Normal = grid.Vertices[i].Normal;
	}

	for(size_t i = 0; i < sphere.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = sphere.Vertices[i].Position;
		vertices[k].Normal = sphere.Vertices[i].Normal;
	}

	for(size_t i = 0; i < cylinder.Vertices.size(); ++i, ++k)
	{
		vertices[k].Pos = cylinder.Vertices[i].Position;
		vertices[k].Normal = cylinder.Vertices[i].Normal;
	}

	for (size_t i = 0; i < diamond.Vertices.size(); ++i, ++k)///////////// //6
	{
		vertices[k].Pos = diamond.Vertices[i].Position;
		vertices[k].Normal = diamond.Vertices[i].Normal;
	}

	for (size_t i = 0; i < pyramid.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = pyramid.Vertices[i].Position;
		vertices[k].Normal = pyramid.Vertices[i].Normal;
	}

	for (size_t i = 0; i < rhombo.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = rhombo.Vertices[i].Position;
		vertices[k].Normal = rhombo.Vertices[i].Normal;
	}

	for (size_t i = 0; i < prism.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = prism.Vertices[i].Position;
		vertices[k].Normal = prism.Vertices[i].Normal;
	}

	for (size_t i = 0; i < hexagon.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = hexagon.Vertices[i].Position;
		vertices[k].Normal = hexagon.Vertices[i].Normal;
	}

	for (size_t i = 0; i < triangleEq.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = triangleEq.Vertices[i].Position;
		vertices[k].Normal = triangleEq.Vertices[i].Normal;
	}	

	for (size_t i = 0; i < triangleRectSqr.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = triangleRectSqr.Vertices[i].Position;
		vertices[k].Normal = triangleRectSqr.Vertices[i].Normal;
	}

	for (size_t i = 0; i < candy1.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = candy1.Vertices[i].Position;
		vertices[k].Normal = candy1.Vertices[i].Normal;
	}

	for (size_t i = 0; i < Box1.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = Box1.Vertices[i].Position;
		vertices[k].Normal = Box1.Vertices[i].Normal;
	}

	for (size_t i = 0; i < Box3.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = Box3.Vertices[i].Position;
		vertices[k].Normal = Box3.Vertices[i].Normal;
	}

	for (size_t i = 0; i < candy2.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = candy2.Vertices[i].Position;
		vertices[k].Normal = candy2.Vertices[i].Normal;
	}

	for (size_t i = 0; i < candy3.Vertices.size(); ++i, ++k)/////////////
	{
		vertices[k].Pos = candy3.Vertices[i].Position;
		vertices[k].Normal = candy3.Vertices[i].Normal;
	}

	std::vector<std::uint16_t> indices;
	indices.insert(indices.end(), std::begin(box.GetIndices16()), std::end(box.GetIndices16()));
	indices.insert(indices.end(), std::begin(grid.GetIndices16()), std::end(grid.GetIndices16()));
	indices.insert(indices.end(), std::begin(sphere.GetIndices16()), std::end(sphere.GetIndices16()));
	indices.insert(indices.end(), std::begin(cylinder.GetIndices16()), std::end(cylinder.GetIndices16()));
	indices.insert(indices.end(), std::begin(diamond.GetIndices16()), std::end(diamond.GetIndices16()));
	indices.insert(indices.end(), std::begin(pyramid.GetIndices16()), std::end(pyramid.GetIndices16()));
	indices.insert(indices.end(), std::begin(rhombo.GetIndices16()), std::end(rhombo.GetIndices16()));///////////// //7
	indices.insert(indices.end(), std::begin(prism.GetIndices16()), std::end(prism.GetIndices16()));
	indices.insert(indices.end(), std::begin(hexagon.GetIndices16()), std::end(hexagon.GetIndices16()));
	indices.insert(indices.end(), std::begin(triangleEq.GetIndices16()), std::end(triangleEq.GetIndices16())); 
	indices.insert(indices.end(), std::begin(triangleRectSqr.GetIndices16()), std::end(triangleRectSqr.GetIndices16()));
	indices.insert(indices.end(), std::begin(candy1.GetIndices16()), std::end(candy1.GetIndices16()));
	indices.insert(indices.end(), std::begin(Box1.GetIndices16()), std::end(Box1.GetIndices16()));
	indices.insert(indices.end(), std::begin(Box3.GetIndices16()), std::end(Box3.GetIndices16()));
	indices.insert(indices.end(), std::begin(candy2.GetIndices16()), std::end(candy2.GetIndices16()));
	indices.insert(indices.end(), std::begin(candy3.GetIndices16()), std::end(candy3.GetIndices16()));


    const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);
    const UINT ibByteSize = (UINT)indices.size()  * sizeof(std::uint16_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "shapeGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R16_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	geo->DrawArgs["box"] = boxSubmesh;
	geo->DrawArgs["grid"] = gridSubmesh;
	geo->DrawArgs["sphere"] = sphereSubmesh;
	geo->DrawArgs["cylinder"] = cylinderSubmesh;
	geo->DrawArgs["diamond"] = diamondSubmesh; ///////////// //8
	geo->DrawArgs["pyramid"] = pyramidSubmesh;
	geo->DrawArgs["rhombo"] = rhomboSubmesh;
	geo->DrawArgs["prism"] = prismSubmesh;
	geo->DrawArgs["hexagon"] = hexagonSubmesh;
	geo->DrawArgs["triangleEq"] = triangleEqSubmesh;
	geo->DrawArgs["triangleRectSqr"] = triangleRectSqrSubmesh;	
	geo->DrawArgs["candy1"] = candy1Submesh;
	geo->DrawArgs["Box1"] = Box1Submesh;
	geo->DrawArgs["Box3"] = Box3Submesh;
	geo->DrawArgs["candy2"] = candy2Submesh;
	geo->DrawArgs["candy3"] = candy3Submesh;

	mGeometries[geo->Name] = std::move(geo);
}

void LitColumnsApp::BuildSkullGeometry()
{
	std::ifstream fin("Models/skull.txt");

	if(!fin)
	{
		MessageBox(0, L"Models/skull.txt not found.", 0, 0);
		return;
	}

	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;

	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;

	std::vector<Vertex> vertices(vcount);
	for(UINT i = 0; i < vcount; ++i)
	{
		fin >> vertices[i].Pos.x >> vertices[i].Pos.y >> vertices[i].Pos.z;
		fin >> vertices[i].Normal.x >> vertices[i].Normal.y >> vertices[i].Normal.z;
	}

	fin >> ignore;
	fin >> ignore;
	fin >> ignore;

	std::vector<std::int32_t> indices(3 * tcount);
	for(UINT i = 0; i < tcount; ++i)
	{
		fin >> indices[i * 3 + 0] >> indices[i * 3 + 1] >> indices[i * 3 + 2];
	}

	fin.close();

	//
	// Pack the indices of all the meshes into one index buffer.
	//

	const UINT vbByteSize = (UINT)vertices.size() * sizeof(Vertex);

	const UINT ibByteSize = (UINT)indices.size() * sizeof(std::int32_t);

	auto geo = std::make_unique<MeshGeometry>();
	geo->Name = "skullGeo";

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &geo->VertexBufferCPU));
	CopyMemory(geo->VertexBufferCPU->GetBufferPointer(), vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &geo->IndexBufferCPU));
	CopyMemory(geo->IndexBufferCPU->GetBufferPointer(), indices.data(), ibByteSize);

	geo->VertexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), vertices.data(), vbByteSize, geo->VertexBufferUploader);

	geo->IndexBufferGPU = d3dUtil::CreateDefaultBuffer(md3dDevice.Get(),
		mCommandList.Get(), indices.data(), ibByteSize, geo->IndexBufferUploader);

	geo->VertexByteStride = sizeof(Vertex);
	geo->VertexBufferByteSize = vbByteSize;
	geo->IndexFormat = DXGI_FORMAT_R32_UINT;
	geo->IndexBufferByteSize = ibByteSize;

	SubmeshGeometry submesh;
	submesh.IndexCount = (UINT)indices.size();
	submesh.StartIndexLocation = 0;
	submesh.BaseVertexLocation = 0;

	geo->DrawArgs["skull"] = submesh;

	mGeometries[geo->Name] = std::move(geo);
}

void LitColumnsApp::BuildPSOs()
{
    D3D12_GRAPHICS_PIPELINE_STATE_DESC opaquePsoDesc;

	//
	// PSO for opaque objects.
	//
    ZeroMemory(&opaquePsoDesc, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
	opaquePsoDesc.InputLayout = { mInputLayout.data(), (UINT)mInputLayout.size() };
	opaquePsoDesc.pRootSignature = mRootSignature.Get();
	opaquePsoDesc.VS = 
	{ 
		reinterpret_cast<BYTE*>(mShaders["standardVS"]->GetBufferPointer()), 
		mShaders["standardVS"]->GetBufferSize()
	};
	opaquePsoDesc.PS = 
	{ 
		reinterpret_cast<BYTE*>(mShaders["opaquePS"]->GetBufferPointer()),
		mShaders["opaquePS"]->GetBufferSize()
	};
	opaquePsoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	opaquePsoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	opaquePsoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	opaquePsoDesc.SampleMask = UINT_MAX;
	opaquePsoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	opaquePsoDesc.NumRenderTargets = 1;
	opaquePsoDesc.RTVFormats[0] = mBackBufferFormat;
	opaquePsoDesc.SampleDesc.Count = m4xMsaaState ? 4 : 1;
	opaquePsoDesc.SampleDesc.Quality = m4xMsaaState ? (m4xMsaaQuality - 1) : 0;
	opaquePsoDesc.DSVFormat = mDepthStencilFormat;
    ThrowIfFailed(md3dDevice->CreateGraphicsPipelineState(&opaquePsoDesc, IID_PPV_ARGS(&mOpaquePSO)));
}

void LitColumnsApp::BuildFrameResources()
{
    for(int i = 0; i < gNumFrameResources; ++i)
    {
        mFrameResources.push_back(std::make_unique<FrameResource>(md3dDevice.Get(),
            1, (UINT)mAllRitems.size(), (UINT)mMaterials.size()));
    }
}

void LitColumnsApp::BuildMaterials()
{
	auto Red = std::make_unique<Material>();
	Red->Name = "Red";
	Red->MatCBIndex = 0;
	Red->DiffuseSrvHeapIndex = 0;
	Red->DiffuseAlbedo = XMFLOAT4(Colors::Red);
	Red->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	Red->Roughness = 0.1f;

	auto Green = std::make_unique<Material>();
	Green->Name = "Green";
	Green->MatCBIndex = 1;
	Green->DiffuseSrvHeapIndex = 1;
	Green->DiffuseAlbedo = XMFLOAT4(Colors::Green);
	Green->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	Green->Roughness = 0.1f;

	auto Blue = std::make_unique<Material>();
	Blue->Name = "Blue";
	Blue->MatCBIndex = 2;
	Blue->DiffuseSrvHeapIndex = 2;
	Blue->DiffuseAlbedo = XMFLOAT4(Colors::Blue);
	Blue->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	Blue->Roughness = 0.1f;

	auto Yellow = std::make_unique<Material>();
	Yellow->Name = "Yellow";
	Yellow->MatCBIndex = 3;
	Yellow->DiffuseSrvHeapIndex = 3;
	Yellow->DiffuseAlbedo = XMFLOAT4(Colors::Yellow);
	Yellow->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	Yellow->Roughness = 0.1f;

	auto Black = std::make_unique<Material>();
	Black->Name = "Black";
	Black->MatCBIndex = 4;
	Black->DiffuseSrvHeapIndex = 4;
	Black->DiffuseAlbedo = XMFLOAT4(Colors::Black);
	Black->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	Black->Roughness = 0.1f;

	auto Brown = std::make_unique<Material>();
	Brown->Name = "Brown";
	Brown->MatCBIndex = 5;
	Brown->DiffuseSrvHeapIndex = 5;
	Brown->DiffuseAlbedo = XMFLOAT4(Colors::Brown);
	Brown->FresnelR0 = XMFLOAT3(0.02f, 0.02f, 0.02f);
	Brown->Roughness = 0.1f;
	
	mMaterials["Red"] = std::move(Red);
	mMaterials["Green"] = std::move(Green);
	mMaterials["Blue"] = std::move(Blue);
	mMaterials["Yellow"] = std::move(Yellow);
	mMaterials["Black"] = std::move(Black);
	mMaterials["Brown"] = std::move(Brown);

	
}

void LitColumnsApp::BuildRenderItems()
{
	/*

		auto hexagonRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&hexagonRitem->World, XMMatrixScaling(3.0f, 0.1f, 3.0f)* XMMatrixTranslation(5.0f, 0.0f, 0.0f));
		XMStoreFloat4x4(&hexagonRitem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		hexagonRitem->ObjCBIndex = 0; //10
		hexagonRitem->Mat = mMaterials["Red"].get();
		hexagonRitem->Geo = mGeometries["shapeGeo"].get();
		hexagonRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		hexagonRitem->IndexCount = hexagonRitem->Geo->DrawArgs["hexagon"].IndexCount;
		hexagonRitem->StartIndexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].StartIndexLocation;
		hexagonRitem->BaseVertexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].BaseVertexLocation;
		mAllRitems.push_back(std::move(hexagonRitem));

		auto hexagonRitem2 = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&hexagonRitem2->World, XMMatrixScaling(3.0f, 0.1f, 3.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		XMStoreFloat4x4(&hexagonRitem2->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		hexagonRitem2->ObjCBIndex = 1; //10
		hexagonRitem2->Mat = mMaterials["Blue"].get();
		hexagonRitem2->Geo = mGeometries["shapeGeo"].get();
		hexagonRitem2->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		hexagonRitem2->IndexCount = hexagonRitem2->Geo->DrawArgs["hexagon"].IndexCount;
		hexagonRitem2->StartIndexLocation = hexagonRitem2->Geo->DrawArgs["hexagon"].StartIndexLocation;
		hexagonRitem2->BaseVertexLocation = hexagonRitem2->Geo->DrawArgs["hexagon"].BaseVertexLocation;
		mAllRitems.push_back(std::move(hexagonRitem2));

		*/
	/*
		auto boxItem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&boxItem->World, XMMatrixScaling(3.0f, 1.0f, 3.0f)* XMMatrixTranslation(0.0f, 1.0f, 0.0f));
		XMStoreFloat4x4(&boxItem->TexTransform, XMMatrixScaling(5.0f, 5.0f, 5.0f));
		boxItem->ObjCBIndex = 0; //10
		boxItem->Mat = mMaterials["Blue"].get();
		boxItem->Geo = mGeometries["shapeGeo"].get();
		boxItem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		boxItem->IndexCount = boxItem->Geo->DrawArgs["box"].IndexCount;
		boxItem->StartIndexLocation = boxItem->Geo->DrawArgs["box"].StartIndexLocation;
		boxItem->BaseVertexLocation = boxItem->Geo->DrawArgs["box"].BaseVertexLocation;
		mAllRitems.push_back(std::move(boxItem));
		
		auto hexagonRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&hexagonRitem->World, XMMatrixScaling(1.0f, 1.0f, 1.0f)* XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		XMStoreFloat4x4(&hexagonRitem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		hexagonRitem->ObjCBIndex = 1; //10
		hexagonRitem->Mat = mMaterials["Red"].get();
		hexagonRitem->Geo = mGeometries["shapeGeo"].get();
		hexagonRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		hexagonRitem->IndexCount = hexagonRitem->Geo->DrawArgs["hexagon"].IndexCount;
		hexagonRitem->StartIndexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].StartIndexLocation;
		hexagonRitem->BaseVertexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].BaseVertexLocation;
		mAllRitems.push_back(std::move(hexagonRitem));
		*/
		auto Box1Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&Box1Ritem->World, XMMatrixScaling(5.0f, 5.0f, 5.0f)* XMMatrixTranslation(15.0f, 0.0f, 0.0f));
		XMStoreFloat4x4(&Box1Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Box1Ritem->ObjCBIndex = 0; //10
		Box1Ritem->Mat = mMaterials["Yellow"].get();
		Box1Ritem->Geo = mGeometries["shapeGeo"].get();
		Box1Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Box1Ritem->IndexCount = Box1Ritem->Geo->DrawArgs["Box1"].IndexCount;
		Box1Ritem->StartIndexLocation = Box1Ritem->Geo->DrawArgs["Box1"].StartIndexLocation;
		Box1Ritem->BaseVertexLocation = Box1Ritem->Geo->DrawArgs["Box1"].BaseVertexLocation;
		mAllRitems.push_back(std::move(Box1Ritem));

		auto Box3Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&Box3Ritem->World, XMMatrixScaling(5.0f, 5.0f, 5.0f) * XMMatrixRotationY(1.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f));
		XMStoreFloat4x4(&Box3Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Box3Ritem->ObjCBIndex = 1; //10
		Box3Ritem->Mat = mMaterials["Red"].get();
		Box3Ritem->Geo = mGeometries["shapeGeo"].get();
		Box3Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Box3Ritem->IndexCount = Box3Ritem->Geo->DrawArgs["Box3"].IndexCount;
		Box3Ritem->StartIndexLocation = Box3Ritem->Geo->DrawArgs["Box3"].StartIndexLocation;
		Box3Ritem->BaseVertexLocation = Box3Ritem->Geo->DrawArgs["Box3"].BaseVertexLocation;
		mAllRitems.push_back(std::move(Box3Ritem));

		auto candy2Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&candy2Ritem->World, XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixRotationY(1.0f) * XMMatrixTranslation(-15.0f, 7.0f, 0.0f));
		XMStoreFloat4x4(&candy2Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		candy2Ritem->ObjCBIndex = 2; //10
		candy2Ritem->Mat = mMaterials["Brown"].get();
		candy2Ritem->Geo = mGeometries["shapeGeo"].get();
		candy2Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		candy2Ritem->IndexCount = candy2Ritem->Geo->DrawArgs["candy2"].IndexCount;
		candy2Ritem->StartIndexLocation = candy2Ritem->Geo->DrawArgs["candy2"].StartIndexLocation;
		candy2Ritem->BaseVertexLocation = candy2Ritem->Geo->DrawArgs["candy2"].BaseVertexLocation;
		mAllRitems.push_back(std::move(candy2Ritem));

		auto candy3Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&candy3Ritem->World, XMMatrixScaling(1.0f, 1.0f, 1.0f) * XMMatrixRotationY(1.0f) * XMMatrixTranslation(15.0f, 7.0f, 0.0f));
		XMStoreFloat4x4(&candy3Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		candy3Ritem->ObjCBIndex = 3; //10
		candy3Ritem->Mat = mMaterials["Green"].get();
		candy3Ritem->Geo = mGeometries["shapeGeo"].get();
		candy3Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		candy3Ritem->IndexCount = candy3Ritem->Geo->DrawArgs["candy3"].IndexCount;
		candy3Ritem->StartIndexLocation = candy3Ritem->Geo->DrawArgs["candy3"].StartIndexLocation;
		candy3Ritem->BaseVertexLocation = candy3Ritem->Geo->DrawArgs["candy3"].BaseVertexLocation;
		mAllRitems.push_back(std::move(candy3Ritem));

		auto Box4Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&Box4Ritem->World, XMMatrixScaling(5.0f, 5.0f, 5.0f)* XMMatrixTranslation(-15.0f, 0.0f, 0.0f));
		XMStoreFloat4x4(&Box4Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		Box4Ritem->ObjCBIndex = 4; //10
		Box4Ritem->Mat = mMaterials["Green"].get();
		Box4Ritem->Geo = mGeometries["shapeGeo"].get();
		Box4Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		Box4Ritem->IndexCount = Box4Ritem->Geo->DrawArgs["Box1"].IndexCount;
		Box4Ritem->StartIndexLocation = Box4Ritem->Geo->DrawArgs["Box1"].StartIndexLocation;
		Box4Ritem->BaseVertexLocation = Box4Ritem->Geo->DrawArgs["Box1"].BaseVertexLocation;
		mAllRitems.push_back(std::move(Box4Ritem));

		auto candy1Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&candy1Ritem->World, XMMatrixScaling(5.0f, 5.0f, 5.0f)* XMMatrixTranslation(0.0f, 7.0f, 0.0f));
		XMStoreFloat4x4(&candy1Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		candy1Ritem->ObjCBIndex = 5; 
		candy1Ritem->Mat = mMaterials["Green"].get();
		candy1Ritem->Geo = mGeometries["shapeGeo"].get();
		candy1Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		candy1Ritem->IndexCount = candy1Ritem->Geo->DrawArgs["candy1"].IndexCount;
		candy1Ritem->StartIndexLocation = candy1Ritem->Geo->DrawArgs["candy1"].StartIndexLocation;
		candy1Ritem->BaseVertexLocation = candy1Ritem->Geo->DrawArgs["candy1"].BaseVertexLocation;
		mAllRitems.push_back(std::move(candy1Ritem));


	UINT objCBIndex = 6; 
	

	//box randomization
	for (int i = 0; i < 100; ++i)
	{

		std::string colour = "Red";

		if (i >= 25)
			colour = "Green";

		if (i >= 50)
			colour = "Blue";

		if (i >= 75)
			colour = "Yellow";


		float Xdir = 1.0f;
		float Ydir = 1.0f;
		float Zdir = 1.0f;
		float radius = 2.8f;

		float HexXcenter = 15.0f; //initial X position of item
		float HexYcenter = 1.0f;  //initial Y position of item
		float HexZcenter = 0.0f;  //initial Z position of item

		Xdir = HexXcenter + radius * static_cast<float>(cos(rand()));
		Ydir = HexYcenter + 1 * static_cast<float>(sin(rand()));
		Zdir = HexZcenter + radius * static_cast<float>(sin(rand()));

		float RotationX = static_cast<float>(sin(rand()));
		float RotationY = static_cast<float>(sin(rand()));
		float RotationZ = static_cast<float>(sin(rand()));
		
		XMMATRIX leftWorld = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(RotationX) * XMMatrixRotationY(RotationY) * XMMatrixRotationZ(RotationZ) * XMMatrixTranslation(Xdir, Ydir, Zdir);
		//XMMATRIX rightWorld = XMMatrixTranslation(+1.0f, 1.5f, -1.0f + i * 5.0f);
		/*
		auto hexagonRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&hexagonRitem->World, leftWorld);
		XMStoreFloat4x4(&hexagonRitem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		hexagonRitem->ObjCBIndex = objCBIndex++;
		hexagonRitem->Mat = mMaterials[colour].get();
		hexagonRitem->Geo = mGeometries["shapeGeo"].get();
		hexagonRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		hexagonRitem->IndexCount = hexagonRitem->Geo->DrawArgs["hexagon"].IndexCount;
		hexagonRitem->StartIndexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].StartIndexLocation;
		hexagonRitem->BaseVertexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].BaseVertexLocation;
		mAllRitems.push_back(std::move(hexagonRitem));
		*/

		auto candy3Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&candy3Ritem->World, leftWorld);
		XMStoreFloat4x4(&candy3Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		candy3Ritem->ObjCBIndex = objCBIndex++; //10
		candy3Ritem->Mat = mMaterials[colour].get();
		candy3Ritem->Geo = mGeometries["shapeGeo"].get();
		candy3Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		candy3Ritem->IndexCount = candy3Ritem->Geo->DrawArgs["candy3"].IndexCount;
		candy3Ritem->StartIndexLocation = candy3Ritem->Geo->DrawArgs["candy3"].StartIndexLocation;
		candy3Ritem->BaseVertexLocation = candy3Ritem->Geo->DrawArgs["candy3"].BaseVertexLocation;
		mAllRitems.push_back(std::move(candy3Ritem));

	}
	
	//box randomization
	for (int i = 0; i < 50; ++i)
	{


		float Xdir = 1.0f;
		float Ydir = 1.0f;
		float Zdir = 1.0f;
		float radius = 2.8f;

		float HexXcenter = -15.0f; //initial X position of item
		float HexYcenter = 1.0f;  //initial Y position of item
		float HexZcenter = 0.0f;  //initial Z position of item

		Xdir = HexXcenter + radius * static_cast<float>(cos(rand()));
		Ydir = HexYcenter + 1.0f * static_cast<float>(sin(rand()));
		Zdir = HexZcenter + radius * static_cast<float>(sin(rand()));

		float RotationX = static_cast<float>(sin(rand()));
		float RotationY = static_cast<float>(sin(rand()));
		float RotationZ = static_cast<float>(sin(rand()));


		XMMATRIX leftWorld = XMMatrixScaling(0.1f, 0.1f, 0.1f) * XMMatrixRotationX(RotationX) * XMMatrixRotationY(RotationY) * XMMatrixRotationZ(RotationZ) * XMMatrixTranslation(Xdir, Ydir, Zdir);;
		//XMMATRIX rightWorld = XMMatrixTranslation(+1.0f, 1.5f, -1.0f + i * 5.0f);
		/*
		auto hexagonRitem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&hexagonRitem->World, leftWorld);
		XMStoreFloat4x4(&hexagonRitem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		hexagonRitem->ObjCBIndex = objCBIndex++;
		hexagonRitem->Mat = mMaterials[colour].get();
		hexagonRitem->Geo = mGeometries["shapeGeo"].get();
		hexagonRitem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		hexagonRitem->IndexCount = hexagonRitem->Geo->DrawArgs["hexagon"].IndexCount;
		hexagonRitem->StartIndexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].StartIndexLocation;
		hexagonRitem->BaseVertexLocation = hexagonRitem->Geo->DrawArgs["hexagon"].BaseVertexLocation;
		mAllRitems.push_back(std::move(hexagonRitem));
		

		auto candy3Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&candy3Ritem->World, leftWorld);
		XMStoreFloat4x4(&candy3Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		candy3Ritem->ObjCBIndex = objCBIndex++; //10
		candy3Ritem->Mat = mMaterials[colour].get();
		candy3Ritem->Geo = mGeometries["shapeGeo"].get();
		candy3Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		candy3Ritem->IndexCount = candy3Ritem->Geo->DrawArgs["candy3"].IndexCount;
		candy3Ritem->StartIndexLocation = candy3Ritem->Geo->DrawArgs["candy3"].StartIndexLocation;
		candy3Ritem->BaseVertexLocation = candy3Ritem->Geo->DrawArgs["candy3"].BaseVertexLocation;
		mAllRitems.push_back(std::move(candy3Ritem));
		*/
		auto candy2Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&candy2Ritem->World, leftWorld);
		XMStoreFloat4x4(&candy2Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		candy2Ritem->ObjCBIndex = objCBIndex++; 
		candy2Ritem->Mat = mMaterials["Brown"].get();
		candy2Ritem->Geo = mGeometries["shapeGeo"].get();
		candy2Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		candy2Ritem->IndexCount = candy2Ritem->Geo->DrawArgs["candy2"].IndexCount;
		candy2Ritem->StartIndexLocation = candy2Ritem->Geo->DrawArgs["candy2"].StartIndexLocation;
		candy2Ritem->BaseVertexLocation = candy2Ritem->Geo->DrawArgs["candy2"].BaseVertexLocation;
		mAllRitems.push_back(std::move(candy2Ritem));


	}


	/*
	//circle randomization
	for (int i = 0; i < 1000; ++i)
	{

		std::string colour = "Red";

		if (i >= 200)
			colour = "Green";

		if (i >= 400)
			colour = "Blue";

		if (i >= 600)
			colour = "Yellow";

		if (i >= 800)
			colour = "Black";

		float RealRadius = 10;
		float r = ((double)rand() / (RAND_MAX)) + 1;
		float a = ((double)rand() / (RAND_MAX)) * XM_2PI;

		float R = RealRadius - sqrt(r) * RealRadius; // Fix distribution and scale to [0, RealRadius]

		float x = cosf(a) * R;
		float y = sinf(a) * R;

		float Xdir = 1.0f;
		float Ydir = 1.0f;
		float Zdir = 1.0f;
		float radius = 2.8;

		float HexXcenter = 0; //initial X position of item
		float HexYcenter = 1; //initial Y position of item
		float HexZcenter = 0; //initial Z position of item

		Xdir = HexXcenter + x;
		Ydir = HexYcenter + 1 * sin(rand());
		Zdir = HexZcenter + y;


		XMMATRIX leftWorld = XMMatrixTranslation(Xdir, Ydir, Zdir);
		//XMMATRIX rightWorld = XMMatrixTranslation(+1.0f, 1.5f, -1.0f + i * 5.0f);


		

		auto candy2Ritem = std::make_unique<RenderItem>();
		XMStoreFloat4x4(&candy2Ritem->World, leftWorld);
		XMStoreFloat4x4(&candy2Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
		candy2Ritem->ObjCBIndex = objCBIndex++; //10
		candy2Ritem->Mat = mMaterials[colour].get();
		candy2Ritem->Geo = mGeometries["shapeGeo"].get();
		candy2Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		candy2Ritem->IndexCount = candy2Ritem->Geo->DrawArgs["candy1"].IndexCount;
		candy2Ritem->StartIndexLocation = candy2Ritem->Geo->DrawArgs["candy1"].StartIndexLocation;
		candy2Ritem->BaseVertexLocation = candy2Ritem->Geo->DrawArgs["candy1"].BaseVertexLocation;
		mAllRitems.push_back(std::move(candy2Ritem));
	}
	
	// All the render items are opaque.
	for(auto& e : mAllRitems)
		mOpaqueRitems.push_back(e.get());
}

*/
	//Triangle randomization
for (int i = 0; i < 100; ++i)
{

	std::string colour = "Red";

	if (i >= 20)
		colour = "Green";

	if (i >= 40)
		colour = "Blue";

	if (i >= 60)
		colour = "Yellow";

	if (i >= 80)
		colour = "Black";

	
	float RealRadius = 17;
	float r = ((float)rand() / (RAND_MAX)) + 1.0f;
	float a = ((float)rand() / (RAND_MAX)) * 1.14f;

	float R = RealRadius - sqrt(r) * RealRadius; // Fix distribution and scale to [0, RealRadius]

	float x = cosf(a) * R;
	float y = sinf(a) * R;
	
	float Xdir = 1.0f;
	float Ydir = 1.0f;
	float Zdir = 1.0f;
	

	float HexXcenter = 2.8f; //initial X position of item
	float HexYcenter = 1.0f; //initial Y position of item
	float HexZcenter = 1.8f; //initial Z position of item

	Xdir = HexXcenter + x;
	Ydir = HexYcenter + 1.0f * static_cast<float>(sin(rand()));
	Zdir = HexZcenter + y;

	
	/*
	float Xdir = 1.0f;
	float Ydir = 1.0f;
	float Zdir = 0.0f;

	float angle = 180;

	float radius = 2.8;

	
	

	Xdir = radius * cos(angle) + Xdir;
	Ydir = radius * sin(angle) + Ydir;

	Xdir = Xdir * cos(rand());
	Ydir = Ydir * sin(rand());
	Zdir = sin(rand());
	*/

	float RotationX = static_cast<float>(sin(rand()));
	float RotationY = static_cast<float>(sin(rand()));
	float RotationZ = static_cast<float>(sin(rand()));
	XMMATRIX leftWorld = XMMatrixRotationX(RotationX) * XMMatrixRotationY(RotationY) * XMMatrixRotationZ(RotationZ) *XMMatrixTranslation(Xdir, Ydir, Zdir);
	//XMMATRIX rightWorld = XMMatrixTranslation(+1.0f, 1.5f, -1.0f + i * 5.0f);



	auto candy2Ritem = std::make_unique<RenderItem>();
	XMStoreFloat4x4(&candy2Ritem->World, leftWorld);
	XMStoreFloat4x4(&candy2Ritem->TexTransform, XMMatrixScaling(1.0f, 1.0f, 1.0f));
	candy2Ritem->ObjCBIndex = objCBIndex++; //10
	candy2Ritem->Mat = mMaterials[colour].get();
	candy2Ritem->Geo = mGeometries["shapeGeo"].get();
	candy2Ritem->PrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	candy2Ritem->IndexCount = candy2Ritem->Geo->DrawArgs["candy1"].IndexCount;
	candy2Ritem->StartIndexLocation = candy2Ritem->Geo->DrawArgs["candy1"].StartIndexLocation;
	candy2Ritem->BaseVertexLocation = candy2Ritem->Geo->DrawArgs["candy1"].BaseVertexLocation;
	mAllRitems.push_back(std::move(candy2Ritem));
}

// All the render items are opaque.
for (auto& e : mAllRitems)
mOpaqueRitems.push_back(e.get());
}

void LitColumnsApp::DrawRenderItems(ID3D12GraphicsCommandList* cmdList, const std::vector<RenderItem*>& ritems)
{
    UINT objCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
    UINT matCBByteSize = d3dUtil::CalcConstantBufferByteSize(sizeof(MaterialConstants));
 
	auto objectCB = mCurrFrameResource->ObjectCB->Resource();
	auto matCB = mCurrFrameResource->MaterialCB->Resource();

    // For each render item...
    for(size_t i = 0; i < ritems.size(); ++i)
    {
        auto ri = ritems[i];

        cmdList->IASetVertexBuffers(0, 1, &ri->Geo->VertexBufferView());
        cmdList->IASetIndexBuffer(&ri->Geo->IndexBufferView());
        cmdList->IASetPrimitiveTopology(ri->PrimitiveType);

        D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = objectCB->GetGPUVirtualAddress() + ri->ObjCBIndex*objCBByteSize;
		D3D12_GPU_VIRTUAL_ADDRESS matCBAddress = matCB->GetGPUVirtualAddress() + ri->Mat->MatCBIndex*matCBByteSize;

        cmdList->SetGraphicsRootConstantBufferView(0, objCBAddress);
		cmdList->SetGraphicsRootConstantBufferView(1, matCBAddress);

        cmdList->DrawIndexedInstanced(ri->IndexCount, 1, ri->StartIndexLocation, ri->BaseVertexLocation, 0);
    }
}
