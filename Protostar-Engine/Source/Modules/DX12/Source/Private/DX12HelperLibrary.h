#pragma once
#include <wrl.h>
#include "d3d12.h"
#include "dxgi1_5.h"
#include "dxgi1_6.h"
#include <chrono>
#include "GlobalDefinitions.h"


#if defined(max)
#undef max
#endif


namespace MWRL = Microsoft::WRL;

namespace Protostar::Rendering
{
	class HelperLibrary
	{
	public:
		static void EnableDebugLayer();
		static MWRL::ComPtr<IDXGIAdapter4> GetAdapter();
		static MWRL::ComPtr<ID3D12Device9> CreateDevice(MWRL::ComPtr<IDXGIAdapter4> _adapter);
		static MWRL::ComPtr<ID3D12CommandQueue> CreateCommandQueue(MWRL::ComPtr<ID3D12Device9> _device, D3D12_COMMAND_LIST_TYPE _type);
		static bool CheckTearingSupport();
		static MWRL::ComPtr<IDXGISwapChain4> CreateSwapChain(HWND _hwnd,
			MWRL::ComPtr<ID3D12CommandQueue> _commandQueue, u32 _width, u32 _height, u32 _bufferCount);
		static MWRL::ComPtr<ID3D12DescriptorHeap> CreateDescriptorHeap(MWRL::ComPtr<ID3D12Device9> _device,
			D3D12_DESCRIPTOR_HEAP_TYPE _type, u32 _numDescriptors);
		static MWRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(MWRL::ComPtr<ID3D12Device> _device,
			D3D12_COMMAND_LIST_TYPE _type);
		static MWRL::ComPtr<ID3D12GraphicsCommandList1> CreateCommandList(MWRL::ComPtr<ID3D12Device9> _device,
			MWRL::ComPtr<ID3D12CommandAllocator> _commandAllocator, D3D12_COMMAND_LIST_TYPE _type);
		static MWRL::ComPtr<ID3D12Fence1> CreateFence(MWRL::ComPtr<ID3D12Device9> _device);
		static HANDLE CreateEventHandle();
		static u64 Signal(MWRL::ComPtr<ID3D12CommandQueue> _commandQueue, MWRL::ComPtr<ID3D12Fence1> _fence,
			u64& _fenceValue);
		static void WaitForFenceValue(MWRL::ComPtr<ID3D12Fence1> _fence, u64 _fenceValue, HANDLE _fenceEvent,
			std::chrono::milliseconds _duration = std::chrono::milliseconds::max());
		static void Flush(MWRL::ComPtr<ID3D12CommandQueue> _commandQueue, MWRL::ComPtr<ID3D12Fence1> _fence,
			u64& _fenceValue, HANDLE _fenceEvent);
	};
}