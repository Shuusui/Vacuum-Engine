#include "DX12.h"
#include "DXHelper.h"
#include "Guid.h"
#include "d3dx12.h"
#include "Window.h"
#include <cassert>
#include <functional>

Protostar::Rendering::DX12::DX12(PSC::Window* _window)
	: IRenderer(_window)
	, m_window(_window)
	, m_fenceValue(0)
	, m_fenceEvent(nullptr)
	, m_rtvDescriptorSize(0)
	, m_currentBackbufferIndex(0)
	, m_vSync(false)
	, m_tearingSupport(false)
{
	_window->RegisterEventCallback(WM_PAINT, std::bind(&DX12::OnWindowPaint, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	HelperLibrary::EnableDebugLayer();

	MWRL::ComPtr<IDXGIAdapter4> dxgiAdapter4 = HelperLibrary::GetAdapter();
	m_device = HelperLibrary::CreateDevice(dxgiAdapter4);
	m_commandQueue = HelperLibrary::CreateCommandQueue(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	m_swapChain = HelperLibrary::CreateSwapChain(_window->GetWindowHandle(), m_commandQueue,
		1920, 1080, NUM_FRAMES);
	m_currentBackbufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	m_rtvDescriptorHeap = HelperLibrary::CreateDescriptorHeap(m_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, NUM_FRAMES);
	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	UpdateRenderTargetViews(m_device, m_swapChain, m_rtvDescriptorHeap);

	for (s32 i = 0; i < NUM_FRAMES; ++i)
	{
		m_commandAllocators[i] = HelperLibrary::CreateCommandAllocator(m_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	m_commandList = HelperLibrary::CreateCommandList(m_device, m_commandAllocators[m_currentBackbufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

	m_fence = HelperLibrary::CreateFence(m_device);
	m_fenceEvent = HelperLibrary::CreateEventHandle();


}

void Protostar::Rendering::DX12::Update()
{

}

void Protostar::Rendering::DX12::Render()
{
	MWRL::ComPtr<ID3D12CommandAllocator> commandAllocator = m_commandAllocators[m_currentBackbufferIndex];
	MWRL::ComPtr<ID3D12Resource1> backBuffer = m_backBuffers[m_currentBackbufferIndex];

	commandAllocator->Reset();
	m_commandList->Reset(commandAllocator.Get(), nullptr);

	CD3DX12_RESOURCE_BARRIER renderTargetBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffer.Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET
	);
	m_commandList->ResourceBarrier(1, &renderTargetBarrier);

	float clearColor[] = { .4f, .6f, .9f, 1.0f };
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(m_rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
		m_currentBackbufferIndex, m_rtvDescriptorSize);

	m_commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);

	CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
		backBuffer.Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	m_commandList->ResourceBarrier(1, &presentBarrier);

	THROW_IF_FAILED(m_commandList->Close());
	ID3D12CommandList* const commandLists[] = {
		m_commandList.Get()
	};
	m_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);
	
	u32 syncInterval = m_vSync ? 1 : 0;
	u32 presentFlags = m_tearingSupport && !m_vSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
	//TODO: use present1 function instead
	THROW_IF_FAILED(m_swapChain->Present(syncInterval, presentFlags));
	m_frameFenceValues[m_currentBackbufferIndex] = HelperLibrary::Signal(m_commandQueue, m_fence, m_fenceValue);

	m_currentBackbufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	HelperLibrary::WaitForFenceValue(m_fence, m_frameFenceValues[m_currentBackbufferIndex], m_fenceEvent);
}

void Protostar::Rendering::DX12::Shutdown()
{
	HelperLibrary::Flush(m_commandQueue, m_fence, m_fenceValue, m_fenceEvent);
	CloseHandle(m_fenceEvent);
}

Protostar::Rendering::DX12::~DX12()
{
	Shutdown();
}

void Protostar::Rendering::DX12::UpdateRenderTargetViews(MWRL::ComPtr<ID3D12Device9> _device, MWRL::ComPtr<IDXGISwapChain4> _swapChain, MWRL::ComPtr<ID3D12DescriptorHeap> _descriptorHeap)
{
	u32 rtvDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(_descriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (s32 i = 0; i < NUM_FRAMES; ++i)
	{
		MWRL::ComPtr<ID3D12Resource1> backBuffer;
		THROW_IF_FAILED(_swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

		_device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

		m_backBuffers[i] = backBuffer;

		rtvHandle.Offset(rtvDescriptorSize);
	}
}

void Protostar::Rendering::DX12::Resize(u32 _width, u32 _height)
{
	if (AreDimensionsSame(_width, _height))
	{
		return;
	}

	HelperLibrary::Flush(m_commandQueue, m_fence, m_fenceValue, m_fenceEvent);

	for (s32 i = 0; i < NUM_FRAMES; ++i)
	{
		m_backBuffers[i].Reset();
		m_frameFenceValues[i] = m_frameFenceValues[m_currentBackbufferIndex];
	}

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	THROW_IF_FAILED(m_swapChain->GetDesc(&swapChainDesc));
	THROW_IF_FAILED(m_swapChain->ResizeBuffers1(NUM_FRAMES, _width, _height,
		swapChainDesc.BufferDesc.Format, swapChainDesc.Flags,
		nullptr, nullptr));

	m_currentBackbufferIndex = m_swapChain->GetCurrentBackBufferIndex();

	UpdateRenderTargetViews(m_device, m_swapChain, m_rtvDescriptorHeap);
}

bool Protostar::Rendering::DX12::AreDimensionsSame(u32 _width, u32 _height)
{
	const Core::Vector2D<s32>& size = m_window->GetCurrentDimensions().Size;
	return size.GetX() == static_cast<s32>(_width) && size.GetY() == static_cast<s32>(_height);
}

s32 Protostar::Rendering::DX12::OnWindowPaint(HWND, u32, WPARAM, LPARAM)
{
	Update();
	Render();
	return 0;
}

