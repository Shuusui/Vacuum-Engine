#pragma once
#include "IRenderer.h"
#include "d3dx12.h"
#include "DirectXMath.h"
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>

namespace Vacuum
{
	struct SVertex
	{
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT4 m_color;
	};

	struct SFrameResource
	{
		Microsoft::WRL::ComPtr<ID3D12Resource> m_indexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		int32 m_indexBufferSize;
		int32 m_vertexBufferSize;
	};

	class DX12Renderer : public IRenderer
	{
		static const uint32 s_frameCount = 3;

	public:
		DX12Renderer(const uint32& _width, const uint32& _height, const HWND& _hwnd)
			:IRenderer(_width, _height, _hwnd)
			,m_hwnd(HWND(_hwnd))
			,m_device(nullptr)
			,m_commandQueue(nullptr)
			,m_swapChain(nullptr)
			,m_rtvHeap(nullptr)
			,m_srvDescHeap(nullptr)
			,m_commandAllocator(nullptr)
			,m_rootSignature(nullptr)
			,m_pipelineState(nullptr)
			,m_commandList(nullptr)
			,m_fence(nullptr)
			,m_vertexBufferView({})
			,m_viewPort(0.0f, 0.0f, static_cast<float>(_width), static_cast<float>(_height))
			,m_scissorRect(0, 0, static_cast<int64>(_width), static_cast<int64>(_height))
			,m_fenceEvent(nullptr)
			,m_frameResources(nullptr)
			,m_frameIndex(0)
			,m_rtvDescriptorSize(0)
			,m_fenceValue(0)
		{

		}

		virtual void OnInit() override;
		virtual void OnUpdate() override;
		virtual void OnRender() override;
		virtual void OnDestroy() override;

	private: 
		void LoadPipeline();
		void LoadAssets();
		void WaitForPreviousFrame();
		void GetHandwareAdapter(struct IDXGIFactory1* _factory, Microsoft::WRL::ComPtr<IDXGIAdapter1>& _adapter);
		void PopulateCommandList();
		void CreateFontsTexture();

		HWND m_hwnd;

		Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_srvDescHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[s_frameCount];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_fontTextureResource;

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		CD3DX12_VIEWPORT m_viewPort;
		CD3DX12_RECT m_scissorRect;
		HANDLE m_fenceEvent;
		SFrameResource* m_frameResources;

		uint32 m_frameIndex;
		uint32 m_rtvDescriptorSize;
		uint64 m_fenceValue;
	};
}