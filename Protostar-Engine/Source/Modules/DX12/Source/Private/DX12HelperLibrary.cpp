#include "DX12HelperLibrary.h"
#include <cassert>
#include "DXHelper.h"


namespace Protostar::Rendering
{
	void HelperLibrary::EnableDebugLayer()
	{
#if defined (_DEBUG)
		MWRL::ComPtr<ID3D12Debug> debugInterface;
		THROW_IF_FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
		debugInterface->EnableDebugLayer();
#endif
	}

	MWRL::ComPtr<IDXGIAdapter4> HelperLibrary::GetAdapter()
	{
		MWRL::ComPtr<IDXGIFactory4> dxgiFactory;
		u32 createFactoryFlags = 0;
#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		THROW_IF_FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory)));
		MWRL::ComPtr<IDXGIAdapter1> dxgiAdapter1;
		MWRL::ComPtr<IDXGIAdapter4> dxgiAdapter4;

		SIZE_T maxDedicatedVideoMemory = 0;
		for (s32 i = 0; dxgiFactory->EnumAdapters1(i, &dxgiAdapter1) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			DXGI_ADAPTER_DESC1 dxgiAdapterDesc1;
			dxgiAdapter1->GetDesc1(&dxgiAdapterDesc1);
			if ((dxgiAdapterDesc1.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) == 0 &&
				SUCCEEDED(D3D12CreateDevice(dxgiAdapter1.Get(),
					D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr)) &&
				dxgiAdapterDesc1.DedicatedVideoMemory > maxDedicatedVideoMemory)
			{
				maxDedicatedVideoMemory = dxgiAdapterDesc1.DedicatedVideoMemory;
				THROW_IF_FAILED(dxgiAdapter1.As(&dxgiAdapter4));
			}
		}
		return dxgiAdapter4;
	}

	MWRL::ComPtr<ID3D12Device9> HelperLibrary::CreateDevice(MWRL::ComPtr<IDXGIAdapter4> _adapter)
	{
		MWRL::ComPtr<ID3D12Device9> d3d12Device9;
		THROW_IF_FAILED(D3D12CreateDevice(_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3d12Device9)));

#if defined(_DEBUG)
		MWRL::ComPtr<ID3D12InfoQueue1> infoQueue;
		if (SUCCEEDED(d3d12Device9.As(&infoQueue)))
		{
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
			infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);
		}
#endif
		return d3d12Device9;
	}

	MWRL::ComPtr<ID3D12CommandQueue> HelperLibrary::CreateCommandQueue(MWRL::ComPtr<ID3D12Device9> device, D3D12_COMMAND_LIST_TYPE type)
	{
		MWRL::ComPtr<ID3D12CommandQueue> d3d12CommandQueue;

		D3D12_COMMAND_QUEUE_DESC commandQueueDesc =
		{
			type,
			D3D12_COMMAND_QUEUE_PRIORITY_NORMAL,
			D3D12_COMMAND_QUEUE_FLAG_NONE,
			0
		};

		//TODO: replace with a better alternative.
		GUID guid2;
		THROW_IF_FAILED(CoCreateGuid(&guid2));
		THROW_IF_FAILED(device->CreateCommandQueue1(&commandQueueDesc, guid2, IID_PPV_ARGS(&d3d12CommandQueue)));
		return d3d12CommandQueue;
	}

	bool HelperLibrary::CheckTearingSupport()
	{
		s32 allowTearing = false;

		MWRL::ComPtr<IDXGIFactory7> factory7;
		u32 createFactoryFlags = 0;
#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		if (SUCCEEDED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&factory7))))
		{
			if (FAILED(factory7->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
			{
				allowTearing = false;
			}
		}
		return allowTearing;
	}

	MWRL::ComPtr<IDXGISwapChain4> HelperLibrary::CreateSwapChain(HWND _hwnd, MWRL::ComPtr<ID3D12CommandQueue> _commandQueue, u32 _width, u32 _height, u32 _bufferCount)
	{
		MWRL::ComPtr<IDXGISwapChain4> dxgiSwapChain4;
		MWRL::ComPtr<IDXGIFactory7> dxgiFactory7;
		u32 createFactoryFlags;
#if defined(_DEBUG)
		createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif
		THROW_IF_FAILED(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory7)));

		DXGI_SWAP_CHAIN_DESC1 swapChainDesc =
		{
			_width,
			_height,
			DXGI_FORMAT_R8G8B8A8_UNORM,
			false,
			{1,0},
			DXGI_USAGE_RENDER_TARGET_OUTPUT,
			_bufferCount,
			DXGI_SCALING_STRETCH,
			DXGI_SWAP_EFFECT_FLIP_DISCARD,
			DXGI_ALPHA_MODE_UNSPECIFIED,
			CheckTearingSupport() ? static_cast<u32>(DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING) : 0
		};

		MWRL::ComPtr<IDXGISwapChain1> dxgiSwapChain1;
		THROW_IF_FAILED(dxgiFactory7->CreateSwapChainForHwnd(
			_commandQueue.Get(),
			_hwnd,
			&swapChainDesc,
			nullptr,
			nullptr,
			&dxgiSwapChain1));

		THROW_IF_FAILED(dxgiFactory7->MakeWindowAssociation(_hwnd, DXGI_MWA_NO_ALT_ENTER));

		THROW_IF_FAILED(dxgiSwapChain1.As(&dxgiSwapChain4));

		return dxgiSwapChain4;
	}

	MWRL::ComPtr<ID3D12DescriptorHeap> HelperLibrary::CreateDescriptorHeap(MWRL::ComPtr<ID3D12Device9> _device, D3D12_DESCRIPTOR_HEAP_TYPE _type, u32 _numDescriptors)
	{
		MWRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

		D3D12_DESCRIPTOR_HEAP_DESC desc =
		{
			_type,
			_numDescriptors
		};

		THROW_IF_FAILED(_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&descriptorHeap)));

		return descriptorHeap;
	}

	MWRL::ComPtr<ID3D12CommandAllocator> HelperLibrary::CreateCommandAllocator(MWRL::ComPtr<ID3D12Device> _device, D3D12_COMMAND_LIST_TYPE _type)
	{
		MWRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
		THROW_IF_FAILED(_device->CreateCommandAllocator(_type, IID_PPV_ARGS(&commandAllocator)));
		return commandAllocator;
	}

	MWRL::ComPtr<ID3D12GraphicsCommandList1> HelperLibrary::CreateCommandList(MWRL::ComPtr<ID3D12Device9> _device, MWRL::ComPtr<ID3D12CommandAllocator> _commandAllocator, D3D12_COMMAND_LIST_TYPE _type)
	{
		MWRL::ComPtr<ID3D12GraphicsCommandList1> commandList;
		THROW_IF_FAILED(_device->CreateCommandList1(0, _type, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&commandList)));

		return commandList;

	}

	MWRL::ComPtr<ID3D12Fence1> HelperLibrary::CreateFence(MWRL::ComPtr<ID3D12Device9> _device)
	{
		MWRL::ComPtr<ID3D12Fence1> fence;

		THROW_IF_FAILED(_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)));

		return fence;
	}

	HANDLE HelperLibrary::CreateEventHandle()
	{
		HANDLE fenceEvent = ::CreateEvent(nullptr, false, false, nullptr);
		assert(fenceEvent && "Failed to create fence event");

		return fenceEvent;
	}

	u64 HelperLibrary::Signal(MWRL::ComPtr<ID3D12CommandQueue> _commandQueue, MWRL::ComPtr<ID3D12Fence1> _fence, u64& _fenceValue)
	{
		u64 fenceValueForSignal = ++_fenceValue;
		THROW_IF_FAILED(_commandQueue->Signal(_fence.Get(), fenceValueForSignal));

		return fenceValueForSignal;
	}

	void HelperLibrary::WaitForFenceValue(MWRL::ComPtr<ID3D12Fence1> _fence, u64 _fenceValue, HANDLE _fenceEvent, std::chrono::milliseconds _duration /*= std::chrono::milliseconds::max()*/)
	{
		if (_fence->GetCompletedValue() < _fenceValue)
		{
			THROW_IF_FAILED(_fence->SetEventOnCompletion(_fenceValue, _fenceEvent));
			::WaitForSingleObject(_fenceEvent, static_cast<DWORD>(_duration.count()));
		}
	}

	void HelperLibrary::Flush(MWRL::ComPtr<ID3D12CommandQueue> _commandQueue, MWRL::ComPtr<ID3D12Fence1> _fence, u64& _fenceValue, HANDLE _fenceEvent)
	{
		u64 fenceValueForSignal = Signal(_commandQueue, _fence, _fenceValue);
		WaitForFenceValue(_fence, fenceValueForSignal, _fenceEvent);
	}
}