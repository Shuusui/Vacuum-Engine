#pragma once
#include "IRenderer.h"
#include "d3dx12.h"
#include "DirectXMath.h"
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include <functional>
//#include "..\..\..\Vacuum-Editor\ImGui\Header Files\Public\imgui.h"

typedef void (*DrawCallback)(); 

namespace Vacuum
{
	struct SFrameResource
	{
		ID3D12Resource* IndexBuffer;
		ID3D12Resource* VertexBuffer;
		int32 IndexBufferSize;
		int32 VertexBufferSize;
	};

	struct SFrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		uint64 FenceValue;
	};

	struct SVertexConstantBuffer
	{
		float MVP[4][4];
	};

	struct S2DVert
	{
		DirectX::XMFLOAT2 Pos;
		DirectX::XMFLOAT2 UV;
		uint32 Col;
	};

	struct S3DVert
	{
		DirectX::XMFLOAT3 Pos; 
		DirectX::XMFLOAT2 UV; 
		uint32 Col;
	};

	struct SGuiDrawCmd
	{
		DirectX::XMFLOAT4 ClipRect;
		void* TextureID;
		uint32 VtxOffset;
		uint32 IdxOffset;
		uint32 ElemCount;
		std::function<void()> UserCallback;
		bool UserCallbackValid = false;
		bool CallUserCallback = false;
	};

	struct SGuiDrawList
	{
		std::vector<S2DVert> VertexBuffer;
		std::vector<unsigned short> IndexBuffer;
		std::vector<SGuiDrawCmd> DrawCommands;
	};

	struct SGuiDrawData
	{
		DirectX::XMFLOAT2 DisplayPos;
		DirectX::XMFLOAT2 DisplaySize;
		int32 TotalIdxCount;
		int32 TotalVtxCount;
		std::vector<SGuiDrawList> DrawLists;
	};


	class DX12Renderer : public IRenderer
	{
		static const uint32 s_frameCount = 3;

	public:
		DX12Renderer(const uint32& _width, const uint32& _height, const HWND& _hwnd, bool& _bVSync)
			:IRenderer(_width, _height, _hwnd, _bVSync)
			,m_hwnd(HWND(_hwnd))
			,m_swapChainWaitableObject(nullptr)
			,m_device(nullptr)
			,m_commandQueue(nullptr)
			,m_swapChain(nullptr)
			,m_rtvHeap(nullptr)
			,m_srvDescHeap(nullptr)
			,m_guiRootSignature(nullptr)
			,m_guiPipelineState(nullptr)
			,m_guiCommandList(nullptr)
			,m_fence(nullptr)
			,m_fontTextureResource(nullptr)
			,m_barrier({})
			,m_renderTargetDescs()
			,m_fenceEvent(nullptr)
			,m_frameResources(nullptr)
			,m_frameContext()
			,m_frameIndex(0)
			,m_fenceValue(0)
		{
		}

		virtual void OnCreate() override;
		virtual void CreateFontsTexture(unsigned char* _pixels, const int32& _width, const int32& _height, uint64& _texID) override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void PrepareRendering() override;
		virtual void UpdateDrawData(SGuiDrawData* _drawData);
		virtual void OnRender() override;
		virtual void OnDestroy() override;
		virtual void RegisterAfterResizeCallback(const std::function<void(HWND, uint32, WPARAM, LPARAM)>& _func) override;
	private:
		template<typename T>
		static void SafeRelease(T*& resource)
		{
			if (resource)
			{
				resource->Release();
			}
			resource = nullptr;
		}

		void LoadPipeline();
		void LoadGUIShaders();
		void LoadAssets();
		void GetHandwareAdapter(struct IDXGIFactory1* _factory, Microsoft::WRL::ComPtr<IDXGIAdapter1>& _adapter);
		SFrameContext* WaitForNextFrameResources();
		void WaitForLastSubmittedFrame();
		void SetupRenderState(SGuiDrawData* _drawData, SFrameResource* _frameResource);
		void SetupViewportRenderState(SGuiDrawData* _drawData);
		void InvalidateObjects();
		void CreateRenderTarget();
		void CleanupRenderTarget();
		void ResizeSwapChain(HWND _hwnd, const uint32& _width, const uint32& _height);

		HWND m_hwnd;
		HANDLE m_swapChainWaitableObject;

		ID3D12Device* m_device;
		ID3D12CommandQueue* m_commandQueue;
		IDXGISwapChain3* m_swapChain;
		ID3D12DescriptorHeap* m_rtvHeap;
		ID3D12DescriptorHeap* m_srvDescHeap;
		ID3D12Resource* m_renderTargets[s_frameCount];
		ID3D12RootSignature* m_guiRootSignature;
		ID3D12PipelineState* m_guiPipelineState;
		ID3D12GraphicsCommandList* m_guiCommandList;
		ID3D12GraphicsCommandList* m_viewPostCommandList;
		ID3D12Fence* m_fence;
		ID3D12Resource* m_fontTextureResource;
		ID3DBlob* m_guiVertexShader;
		ID3DBlob* m_guiPixelShader;

		D3D12_RESOURCE_BARRIER m_barrier;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_renderTargetDescs[s_frameCount];
		HANDLE m_fenceEvent;
		SFrameResource* m_frameResources;
		SFrameContext m_frameContext[s_frameCount];

		uint32 m_frameIndex;
		uint64 m_fenceValue;
		std::vector<std::function<void(HWND, uint32, WPARAM, LPARAM)>> m_afterResizeCallbacks;
	};
}