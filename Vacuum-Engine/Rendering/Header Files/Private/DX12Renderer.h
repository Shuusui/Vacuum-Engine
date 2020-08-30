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
		s32 IndexBufferSize;
		s32 VertexBufferSize;
	};

	struct SFrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		u64 FenceValue;
	};

	struct SVertexConstantBuffer
	{
		float MVP[4][4];
	};

	struct S2DVert
	{
		DirectX::XMFLOAT2 Pos;
		DirectX::XMFLOAT2 UV;
		u32 Col;
	};

	struct S3DVert
	{
		DirectX::XMFLOAT3 Pos; 
		DirectX::XMFLOAT2 UV; 
		u32 Col;
	};

	struct SGuiDrawCmd
	{
		DirectX::XMFLOAT4 ClipRect;
		void* TextureID;
		u32 VtxOffset;
		u32 IdxOffset;
		u32 ElemCount;
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
		s32 TotalIdxCount;
		s32 TotalVtxCount;
		std::vector<SGuiDrawList> DrawLists;
	};

	struct SDrawCmd
	{
		DirectX::XMFLOAT4 ClipRect;
		u32 VtxOffset;
		u32 IdxOffset;
		u32 ElemCount;
	};

	struct SDrawList
	{
		std::vector<S3DVert> VertexBuffer;
		std::vector<unsigned short> IndexBuffer;
		std::vector<SDrawCmd> DrawCmds;
	};

	struct SDrawData
	{
		DirectX::XMFLOAT2 DisplayPos;
		DirectX::XMFLOAT2 DisplaySize;
		s32 TotalIdxCount;
		s32 TotalVtxCount;
		std::vector<SDrawList> DrawLists;
	};


	class DX12Renderer : public IRenderer
	{
		static const u32 s_frameCount = 3;

	public:
		DX12Renderer(const u32& _width, const u32& _height, const HWND& _hwnd, bool& _bVSync)
			:IRenderer(_width, _height, _hwnd, _bVSync)
			,m_hwnd(HWND(_hwnd))
			,m_swapChainWaitableObject(nullptr)
			,m_device(nullptr)
			,m_commandQueue(nullptr)
			,m_swapChain(nullptr)
			,m_rtvHeap(nullptr)
			,m_srvDescHeap(nullptr)
			,m_renderTargets()
			,m_guiRootSignature(nullptr)
			,m_guiPipelineState(nullptr)
			,m_guiCommandList(nullptr)
			,m_viewPortCommandList(nullptr)
			,m_fence(nullptr)
			,m_fontTextureResource(nullptr)
			,m_guiVertexShader(nullptr)
			,m_guiPixelShader(nullptr)
			,m_barrier({})
			,m_renderTargetDescs()
			,m_fenceEvent(nullptr)
			,m_frameResources(nullptr)
			,m_frameContext()
			,m_frameIndex(0)
			,m_fenceValue(0)
		{
			m_renderTargets[0] = nullptr;
			m_renderTargets[1] = nullptr;
			m_renderTargets[2] = nullptr;
		}

		virtual void OnCreate() override;
		virtual void CreateFontsTexture(unsigned char* _pixels, const s32& _width, const s32& _height, u64& _texID) override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void PrepareRendering() override;
		virtual void UpdateDrawData(SGuiDrawData* _drawData);
		virtual void UpdateVPDrawData(SDrawData* _drawData);
		virtual void OnRender() override;
		virtual void OnDestroy() override;
		virtual void RegisterAfterResizeCallback(const std::function<void(HWND, u32, WPARAM, LPARAM)>& _func) override;
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
		void SetupViewportRenderState(SDrawData* _drawData, SFrameResource* _frameResource);
		void InvalidateObjects();
		void CreateRenderTarget();
		void CleanupRenderTarget();
		void ResizeSwapChain(HWND _hwnd, const u32& _width, const u32& _height);

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
		ID3D12GraphicsCommandList* m_vpCommandList;
		ID3D12GraphicsCommandList* m_viewPortCommandList;
		ID3D12Fence* m_fence;
		ID3D12Resource* m_fontTextureResource;
		ID3DBlob* m_guiVertexShader;
		ID3DBlob* m_guiPixelShader;

		D3D12_RESOURCE_BARRIER m_barrier;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_renderTargetDescs[s_frameCount];
		HANDLE m_fenceEvent;
		SFrameResource* m_frameResources;
		SFrameContext m_frameContext[s_frameCount];

		u32 m_frameIndex;
		u64 m_fenceValue;
		std::vector<std::function<void(HWND, u32, WPARAM, LPARAM)>> m_afterResizeCallbacks;
	};
}