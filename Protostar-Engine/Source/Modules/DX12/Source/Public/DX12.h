#pragma once
#include "IRenderer.h"
#include "GlobalConstants.h"
#include "DX12HelperLibrary.h"

#ifdef DX12_LIBRARY
	#define DX12_API __declspec(dllexport)
#else
	#define DX12_API __declspec(dllimport)
#endif

namespace Protostar::Core
{
	class Window;
}

namespace PSC = Protostar::Core;

namespace Protostar::Rendering
{

	class DX12_API DX12 : public IRenderer
	{
	public:
		DX12(PSC::Window* _window);
		virtual void Update() override;
		virtual void Render() override;
		virtual void Shutdown() override;
		virtual ~DX12() override;
	private:
		void UpdateRenderTargetViews(MWRL::ComPtr<ID3D12Device9> _device,
			MWRL::ComPtr<IDXGISwapChain4> _swapChain, MWRL::ComPtr<ID3D12DescriptorHeap> _descriptorHeap);
		void Resize(u32 _width, u32 _height);
		bool AreDimensionsSame(u32 _width, u32 _height);
		s32 OnWindowPaint(HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam);

		static constexpr u8 NUM_FRAMES = 3;
		MWRL::ComPtr<ID3D12Device9> m_device;
		MWRL::ComPtr<ID3D12CommandQueue> m_commandQueue;
		MWRL::ComPtr<IDXGISwapChain4> m_swapChain;
		MWRL::ComPtr<ID3D12Resource1> m_backBuffers[NUM_FRAMES];
		MWRL::ComPtr<ID3D12GraphicsCommandList> m_commandList;
		MWRL::ComPtr<ID3D12CommandAllocator> m_commandAllocators[NUM_FRAMES];
		MWRL::ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;
		MWRL::ComPtr<ID3D12Fence1> m_fence;
		PSC::Window* m_window;
		u64 m_frameFenceValues[NUM_FRAMES] = {0,0,0};
		u64 m_fenceValue;
		HANDLE m_fenceEvent;
		u32 m_rtvDescriptorSize;
		u32 m_currentBackbufferIndex;
		bool m_vSync;
		bool m_tearingSupport;
	};
}