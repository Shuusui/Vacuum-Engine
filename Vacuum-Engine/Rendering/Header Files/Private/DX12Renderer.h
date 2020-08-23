#pragma once
#include "IRenderer.h"
#include "d3dx12.h"
#include "DirectXMath.h"
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include "..\..\..\Vacuum-Editor\ImGui\Header Files\Public\imgui.h"

namespace Vacuum
{
	struct SVertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT4 Color;
	};

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
			,m_rootSignature(nullptr)
			,m_pipelineState(nullptr)
			,m_commandList(nullptr)
			,m_fence(nullptr)
			,m_fontTextureResource(nullptr)
			,m_vertexBufferView({})
			,m_barrier({})
			,m_renderTargetDescs()
			,m_viewPort(0.0f, 0.0f, static_cast<float>(_width), static_cast<float>(_height))
			,m_scissorRect(0, 0, static_cast<int64>(_width), static_cast<int64>(_height))
			,m_fenceEvent(nullptr)
			,m_frameResources(nullptr)
			,m_frameContext()
			,m_frameIndex(0)
			,m_rtvDescriptorSize(0)
			,m_fenceValue(0)
		{

		}

		virtual void OnCreate() override;
		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void PrepareRendering() override;
		virtual void OnRender() override;
		virtual void OnDestroy() override;

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
		void LoadAssets();
		void GetHandwareAdapter(struct IDXGIFactory1* _factory, Microsoft::WRL::ComPtr<IDXGIAdapter1>& _adapter);
		void CreateFontsTexture();
		SFrameContext* WaitForNextFrameResources();
		void WaitForLastSubmittedFrame();
		void SetupRenderState(struct ImDrawData* _drawData, SFrameResource* _frameResource);
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
		ID3D12RootSignature* m_rootSignature;
		ID3D12PipelineState* m_pipelineState;
		ID3D12GraphicsCommandList* m_commandList;
		ID3D12Fence* m_fence;
		ID3D12Resource* m_fontTextureResource;

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		D3D12_RESOURCE_BARRIER m_barrier;
		CD3DX12_CPU_DESCRIPTOR_HANDLE m_renderTargetDescs[s_frameCount];
		CD3DX12_VIEWPORT m_viewPort;
		CD3DX12_RECT m_scissorRect;
		HANDLE m_fenceEvent;
		SFrameResource* m_frameResources;
		SFrameContext m_frameContext[s_frameCount];

		uint32 m_frameIndex;
		uint32 m_rtvDescriptorSize;
		uint64 m_fenceValue;
	};
}