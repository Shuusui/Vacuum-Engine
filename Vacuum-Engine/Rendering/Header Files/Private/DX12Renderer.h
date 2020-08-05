#pragma once
#include "IRenderer.h"
#include "d3dx12.h"
#include <dxgi1_2.h>
#include <dxgi1_4.h>
#include <dxgi1_6.h>
#include "DirectXMath.h"

namespace Vacuum
{
	class DX12Renderer : public IRenderer
	{
		struct SVertex
		{
			DirectX::XMFLOAT3 m_position;
			DirectX::XMFLOAT4 m_color;
		};

	public:
		DX12Renderer(const uint32& _width, const uint32& _height, const HWND& _hwnd)
			:IRenderer(_width, _height, _hwnd)
			,m_hwnd(HWND(_hwnd))
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

		static const uint32 s_frameCount = 2;
		HWND m_hwnd;

		Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[s_frameCount];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12RootSignature> m_rootSignature;
		Microsoft::WRL::ComPtr<ID3D12PipelineState> m_pipelineState;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D12Fence> m_fence;

		D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
		CD3DX12_VIEWPORT m_viewPort;
		CD3DX12_RECT m_scissorRect;
		HANDLE m_fenceEvent;

		uint32 m_frameIndex;
		uint32 m_rtvDescriptorSize;
		uint64 m_fenceValue;
	};
}