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
		void GetHandwareAdapter(struct IDXGIFactory1* _factory, Microsoft::WRL::ComPtr<IDXGIAdapter1>& _adapter);

		static const uint32 s_frameCount = 2;
		HWND m_hwnd;
		Microsoft::WRL::ComPtr<ID3D12Device> m_device;
		Microsoft::WRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		Microsoft::WRL::ComPtr<IDXGISwapChain3> m_swapChain;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
		Microsoft::WRL::ComPtr<ID3D12Resource> m_renderTargets[s_frameCount];
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> m_commandAllocator;

		uint32 m_frameIndex;
		uint32 m_rtvDescriptorSize;
	};
}