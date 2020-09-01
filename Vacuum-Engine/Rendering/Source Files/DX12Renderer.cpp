#include "..\Header Files\Private\DX12Renderer.h"

#pragma region Internal Includes 
#include "..\Header Files\Private\DXHelper.h"
#include "Window.h"
#include "SharedStructs.h"
#pragma endregion //Internal Includes
#pragma region External Includes
#include <d3d12sdklayers.h>
#include <d3dcompiler.h>
#include <functional>
#pragma endregion //External Includes

using namespace Microsoft::WRL;

#ifdef _DEBUG
#define DX12_ENABLE_DEBUG_LAYER
#endif 

#ifdef DX12_ENABLE_DEBUG_LAYER
#include <dxgidebug.h>
#pragma comment(lib, "dxguid.lib")
#endif

#define DX_OFFSETOF(_TYPE,_MEMBER)  ((size_t)&(((_TYPE*)0)->_MEMBER))   

void Vacuum::DX12Renderer::OnCreate()
{
	CMainWindow* mainWindow =  CMainWindow::GetWindowHandle();
	if (!mainWindow)
	{
		return;
	}
	LoadPipeline();

	std::function<s32(HWND, u32, WPARAM, LPARAM)> func([this](HWND _hwnd, u32 _msg, WPARAM _wParam, LPARAM _lParam)->s32
	{
			if (m_device != nullptr && _wParam != SIZE_MINIMIZED)
			{
				WaitForLastSubmittedFrame();
				InvalidateObjects();
				CleanupRenderTarget();
				ResizeSwapChain(_hwnd, (u32)LOWORD(_lParam), (u32)HIWORD(_lParam));
				CreateRenderTarget();
				LoadAssets();
				LoadVPAssets();
				for(const std::function<void(HWND, u32, WPARAM, LPARAM)>& func : m_afterResizeCallbacks)
				{
					func(_hwnd, _msg, _wParam, _lParam);
				}
			}
			return 0;
	});

	mainWindow->RegisterCallbackForWMEvents(WM_SIZE, func);
}

void Vacuum::DX12Renderer::OnInit()
{
	LoadAssets();
}

void Vacuum::DX12Renderer::OnUpdate()
{
	m_frameIndex = m_frameIndex + 1;
	SFrameResources* frameResources = &m_frameResources[m_frameIndex % s_frameCount];

	void* vtxResource = nullptr;
	void* idxResource = nullptr;
	D3D12_RANGE range = {};

	if (frameResources->GUIFrameResources->VertexBuffer == nullptr || frameResources->GUIFrameResources->VertexBufferSize < m_guiVertexCountNextTick)
	{
		SafeRelease(frameResources->GUIFrameResources->VertexBuffer);
		frameResources->GUIFrameResources->VertexBufferSize = m_guiVertexCountNextTick + 5000;

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = frameResources->GUIFrameResources->VertexBufferSize * sizeof(S2DVert);
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResources->GUIFrameResources->VertexBuffer)) < 0)
		{
			return;
		}
	}
	if (frameResources->GUIFrameResources->IndexBuffer == nullptr || frameResources->GUIFrameResources->IndexBufferSize < m_guiIdxCountNextTick)
	{
		SafeRelease(frameResources->GUIFrameResources->IndexBuffer);
		frameResources->GUIFrameResources->IndexBufferSize = m_guiIdxCountNextTick + 10000;

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = frameResources->GUIFrameResources->IndexBufferSize * sizeof(u16);
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResources->GUIFrameResources->IndexBuffer)) < 0)
		{
			return;
		}
	}

	frameResources->GUIFrameResources->VertexBuffer->Map(0, &range, &vtxResource);
	frameResources->GUIFrameResources->IndexBuffer->Map(0, &range, &idxResource);

	for(SGuiDrawData* guiDrawData : m_guiDrawDatas)
	{
		for (const SGuiDrawList& guiDrawList : guiDrawData->DrawLists)
		{
			S2DVert* vtxDest = (S2DVert*)vtxResource;
			u16* idxDest = (u16*)idxResource;

			memcpy(vtxDest, guiDrawList.VertexBuffer.data(), guiDrawList.VertexBuffer.size() * sizeof(S2DVert));
			memcpy(idxDest, guiDrawList.IndexBuffer.data(), guiDrawList.IndexBuffer.size() * sizeof(u16));

			vtxDest += guiDrawList.VertexBuffer.size();
			idxDest += guiDrawList.IndexBuffer.size();
		}
	}

	frameResources->GUIFrameResources->VertexBuffer->Unmap(0, &range);
	frameResources->GUIFrameResources->IndexBuffer->Unmap(0, &range);

	for (SGuiDrawData* guiDrawData : m_guiDrawDatas)
	{
		DirectX::XMFLOAT2 clipOff = guiDrawData->DisplayPos;

		SetupRenderState(guiDrawData, frameResources->GUIFrameResources);

		s32 vtxOffset = 0;
		s32 idxOffset = 0;

		for (const SGuiDrawList& drawList : guiDrawData->DrawLists)
		{
			for (const SGuiDrawCmd& drawCmd : drawList.DrawCommands)
			{
				if (drawCmd.UserCallbackValid)
				{
					if (!drawCmd.CallUserCallback)
					{
						SetupRenderState(guiDrawData, frameResources->GUIFrameResources);
					}
					else
					{
						drawCmd.UserCallback();
					}
				}
				else
				{
					const D3D12_RECT rect = { (LONG)(drawCmd.ClipRect.x, - clipOff.x), (LONG)(drawCmd.ClipRect.y - clipOff.y), (LONG)(drawCmd.ClipRect.z - clipOff.x), (LONG)(drawCmd.ClipRect.w - clipOff.y) };
					m_guiCommandList->SetGraphicsRootDescriptorTable(1, *(D3D12_GPU_DESCRIPTOR_HANDLE*)&drawCmd.TextureID);
					m_guiCommandList->RSSetScissorRects(1, &rect);
					m_guiCommandList->DrawIndexedInstanced(drawCmd.ElemCount, 1, drawCmd.IdxOffset + idxOffset, drawCmd.VtxOffset + vtxOffset, 0);
				}
			}

			idxOffset += drawList.IndexBuffer.size();
			vtxOffset += drawList.VertexBuffer.size();
		}
	}


	if (frameResources->VPFrameResources->VertexBuffer == nullptr || frameResources->VPFrameResources->VertexBufferSize < m_vpVertexCountNextTick)
	{
		SafeRelease(frameResources->VPFrameResources->VertexBuffer);
		frameResources->VPFrameResources->VertexBufferSize = m_vpVertexCountNextTick + 5000;

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = frameResources->VPFrameResources->VertexBufferSize * sizeof(SVertex);
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResources->VPFrameResources->VertexBuffer)) < 0)
		{
			return;
		}
	}
	if (frameResources->VPFrameResources->IndexBuffer == nullptr || frameResources->VPFrameResources->IndexBufferSize < m_vpIdxCountNextTick)
	{
		SafeRelease(frameResources->VPFrameResources->IndexBuffer);
		frameResources->VPFrameResources->IndexBufferSize = m_vpIdxCountNextTick + 10000;

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = frameResources->VPFrameResources->IndexBufferSize * sizeof(u16);
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResources->VPFrameResources->IndexBuffer)) < 0)
		{
			return;
		}
	}

	frameResources->VPFrameResources->VertexBuffer->Map(0, &range, &vtxResource);
	frameResources->VPFrameResources->IndexBuffer->Map(0, &range, &idxResource);

	for(SDrawData* drawData : m_drawDatas)
	{
		for(const SDrawList& drawList : drawData->DrawLists)
		{
			SVertex* vtxDest = (SVertex*)vtxResource;
			u16* idxDest = (u16*)idxResource;

			memcpy(vtxDest, drawList.VertexBuffer.data(), drawList.VertexBuffer.size() * sizeof(SVertex));
			memcpy(idxDest, drawList.IndexBuffer.data(), drawList.IndexBuffer.size() * sizeof(u16));

			vtxDest += drawList.VertexBuffer.size();
			idxDest += drawList.IndexBuffer.size();
		}
	}

	frameResources->VPFrameResources->VertexBuffer->Unmap(0, &range);
	frameResources->VPFrameResources->IndexBuffer->Unmap(0, &range);

	for(SDrawData* drawData : m_drawDatas)
	{
		DirectX::XMFLOAT2 clipOff = drawData->DisplayPos;

		s32 vtxOffset = 0;
		s32 idxOffset = 0;

		SetupViewportRenderState(drawData, frameResources->VPFrameResources);

		for (const SDrawList& drawList : drawData->DrawLists)
		{
			for (const SDrawCmd& drawCmd : drawList.DrawCmds)
			{
				const D3D12_RECT rect = { (LONG)(drawCmd.ClipRect.x, - clipOff.x), (LONG)(drawCmd.ClipRect.y - clipOff.y), (LONG)(drawCmd.ClipRect.z - clipOff.x), (LONG)(drawCmd.ClipRect.w - clipOff.y) };
				m_vpCommandList->RSSetScissorRects(1, &rect);
				m_vpCommandList->DrawIndexedInstanced(drawCmd.ElemCount, 1, drawCmd.IdxOffset + idxOffset, drawCmd.VtxOffset + vtxOffset, 0);
			}

			idxOffset += drawList.IndexBuffer.size();
			vtxOffset += drawList.VertexBuffer.size();
		}
	}


	m_guiBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_guiBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	m_guiCommandList->ResourceBarrier(1, &m_guiBarrier);
	m_guiCommandList->Close();

	m_vpBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_vpBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	m_vpCommandList->ResourceBarrier(1, &m_vpBarrier);
	m_vpCommandList->Close();

	m_guiVertexCountNextTick = 0;
	m_vpVertexCountNextTick = 0;

	m_guiIdxCountNextTick = 0;
	m_vpIdxCountNextTick = 0;

	m_guiVertexBufferSizeNextTick = 0;
	m_vpVertexBufferSizeNextTick = 0;

	m_guiIdxBufferSizeNextTick = 0;
	m_vpIdxBufferSizeNextTick = 0;
}

void Vacuum::DX12Renderer::PrepareRendering()
{
	if (!m_guiPipelineState)
	{
		LoadAssets();
	}

	if(!m_vpPipelineState)
	{
		LoadVPAssets();
	}

	SFrameContext* frameCtx = WaitForNextFrameResources();
	u32 backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	frameCtx->GUICommandAllocator->Reset();
	VE_LOG("gui command allocator reset");
	frameCtx->VPCommandAllocator->Reset();
	VE_LOG("vp command allocator reset");

	m_guiBarrier = {};
	m_guiBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_guiBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_guiBarrier.Transition.pResource = m_renderTargets[backBufferIndex];
	m_guiBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_guiBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_guiBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	m_vpBarrier = {};
	m_vpBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_vpBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_vpBarrier.Transition.pResource = m_renderTargets[backBufferIndex];
	m_vpBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_vpBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_vpBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;


	const float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	THROW_IF_FAILED(m_guiCommandList->Reset(frameCtx->GUICommandAllocator, nullptr));
	VE_LOG("gui command list reset");
	m_guiCommandList->ResourceBarrier(1, &m_guiBarrier);
	m_guiCommandList->ClearRenderTargetView(m_renderTargetDescs[backBufferIndex], clearColor, 0, nullptr);
	m_guiCommandList->OMSetRenderTargets(1, &m_renderTargetDescs[backBufferIndex], FALSE, nullptr);
	m_guiCommandList->SetDescriptorHeaps(1, &m_srvDescHeap);


	THROW_IF_FAILED(m_vpCommandList->Reset(frameCtx->VPCommandAllocator, nullptr));
	VE_LOG("vp command list reset");
	m_vpCommandList->ResourceBarrier(1, &m_vpBarrier);
	//m_vpCommandList->ClearRenderTargetView(m_renderTargetDescs[backBufferIndex], redClearColor, 0, nullptr);
	m_vpCommandList->OMSetRenderTargets(1, &m_renderTargetDescs[backBufferIndex], FALSE, nullptr);
	m_vpCommandList->SetDescriptorHeaps(1, &m_vpSrvDescHeap);
}

void Vacuum::DX12Renderer::UpdateDrawData(SGuiDrawData* _drawData)
{
	if (_drawData->DisplaySize.x <= 0.0f || _drawData->DisplaySize.y <= 0.0f)
	{
		return;
	}

	m_guiDrawDatas.push_back(_drawData);

	m_guiVertexCountNextTick += _drawData->TotalVtxCount;
	m_guiIdxCountNextTick += _drawData->TotalIdxCount;

	m_guiVertexBufferSizeNextTick += _drawData->TotalVtxCount * sizeof(S2DVert);
	m_guiIdxBufferSizeNextTick += _drawData->TotalIdxCount * sizeof(u16);
}

void Vacuum::DX12Renderer::UpdateVPDrawData(SDrawData* _drawData)
{
	if (_drawData->DisplaySize.x <= 0.0f || _drawData->DisplaySize.y <= 0.0f)
	{
		return;
	}

	m_drawDatas.push_back(_drawData);

	m_vpVertexCountNextTick += _drawData->TotalVtxCount;
	m_vpIdxCountNextTick += _drawData->TotalIdxCount;

	m_vpVertexBufferSizeNextTick += _drawData->TotalVtxCount * sizeof(SVertex);
	m_vpVertexBufferSizeNextTick += _drawData->TotalIdxCount * sizeof(u16);
}

void Vacuum::DX12Renderer::OnRender()
{
	std::vector<ID3D12CommandList*> commandLists = std::vector<ID3D12CommandList*>(2);
	commandLists[0] = m_guiCommandList;
	commandLists[1] = m_vpCommandList;

	m_commandQueue->ExecuteCommandLists(2, commandLists.data());
	VE_LOG("Execute command lists");
	m_swapChain->Present(m_bVSync, 0);

	u64 fenceValue = m_fenceValue + 1;
	m_commandQueue->Signal(m_fence, fenceValue);
	m_fenceValue = fenceValue;

	SFrameContext* frameCtx = &m_frameContext[m_frameIndex % s_frameCount];
	frameCtx->FenceValue = fenceValue;
}

void Vacuum::DX12Renderer::OnDestroy()
{
	InvalidateObjects();

	delete[] m_frameResources;
	m_frameResources = nullptr;
	m_device = nullptr;
	m_frameIndex = UINT_MAX;

	CleanupRenderTarget();
	SafeRelease(m_swapChain);
	if(m_swapChainWaitableObject) CloseHandle(m_swapChainWaitableObject);
	for (u32 i = 0; i < s_frameCount; ++i)
	{
		SafeRelease(m_frameContext[i].GUICommandAllocator);
		SafeRelease(m_frameContext[i].VPCommandAllocator);
	}
	SafeRelease(m_commandQueue);
	SafeRelease(m_guiCommandList);
	SafeRelease(m_vpCommandList);
	SafeRelease(m_srvDescHeap);
	SafeRelease(m_vpSrvDescHeap);
	SafeRelease(m_rtvHeap);
	SafeRelease(m_fence);
	SafeRelease(m_guiVertexShader);
	SafeRelease(m_vpVertexShader);
	SafeRelease(m_guiPixelShader);
	SafeRelease(m_vpPixelShader);
	if(m_fenceEvent) CloseHandle(m_fenceEvent); m_fenceEvent = nullptr;
	SafeRelease(m_device);

#ifdef DX12_ENABLE_DEBUG_LAYER
	IDXGIDebug1* debug = nullptr;
	if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(&debug))))
	{
		debug->ReportLiveObjects(DXGI_DEBUG_ALL, DXGI_DEBUG_RLO_SUMMARY);
		debug->Release();
	}
#endif
}

void Vacuum::DX12Renderer::RegisterAfterResizeCallback(const std::function<void(HWND, u32, WPARAM, LPARAM)>& _func)
{
	m_afterResizeCallbacks.push_back(_func);
}

void Vacuum::DX12Renderer::LoadPipeline()
{
#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	m_frameResources = new SFrameResources[s_frameCount];

	for (s32 i = 0; i < s_frameCount; ++i)
	{
		m_frameResources[i].GUIFrameResources = new SFrameResource();
		m_frameResources[i].VPFrameResources = new SFrameResource();

		SFrameResources* frameResources = &m_frameResources[i];

		frameResources->GUIFrameResources->IndexBuffer = nullptr;
		frameResources->GUIFrameResources->VertexBuffer = nullptr;
		frameResources->GUIFrameResources->IndexBufferSize = 10000;
		frameResources->GUIFrameResources->VertexBufferSize = 5000;

		frameResources->VPFrameResources->IndexBuffer = nullptr;
		frameResources->VPFrameResources->VertexBuffer = nullptr;
		frameResources->VPFrameResources->IndexBufferSize = 10000;
		frameResources->VPFrameResources->VertexBufferSize = 5000;
	}

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
	THROW_IF_FAILED(D3D12CreateDevice(nullptr, featureLevel, IID_PPV_ARGS(&m_device)));

	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		desc.NumDescriptors = s_frameCount;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		desc.NodeMask = 1;
		THROW_IF_FAILED(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_rtvHeap)));

		SIZE_T rtvDescSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();

		for (u32 i = 0; i < s_frameCount; ++i)
		{
			m_renderTargetDescs[i] = rtvHandle;
			rtvHandle.ptr += rtvDescSize;
		}
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		THROW_IF_FAILED(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_srvDescHeap)));
	}

	{
		D3D12_DESCRIPTOR_HEAP_DESC desc = {};
		desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		desc.NumDescriptors = 1;
		desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		THROW_IF_FAILED(m_device->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&m_vpSrvDescHeap)));
	}

	{
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 1;
		THROW_IF_FAILED(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue)));
	}

	for (s32 i = 0; i < s_frameCount; ++i)
	{
		THROW_IF_FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_frameContext[i].GUICommandAllocator)));
		THROW_IF_FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_frameContext[i].VPCommandAllocator)));
	}

	THROW_IF_FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_frameContext[0].GUICommandAllocator, nullptr, IID_PPV_ARGS(&m_guiCommandList)));


	THROW_IF_FAILED(m_guiCommandList->Close());

	THROW_IF_FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_frameContext[0].VPCommandAllocator, nullptr, IID_PPV_ARGS(&m_vpCommandList)));

	THROW_IF_FAILED(m_vpCommandList->Close());

	THROW_IF_FAILED(m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence)));

	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = s_frameCount;
		swapChainDesc.Width = 0;
		swapChainDesc.Height = 0;
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
		swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
		swapChainDesc.Stereo = FALSE;

		ComPtr<IDXGIFactory4> dxgiFactory = nullptr;
		ComPtr<IDXGISwapChain1> swapChain = nullptr;
		THROW_IF_FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&dxgiFactory)));
		THROW_IF_FAILED(dxgiFactory->CreateSwapChainForHwnd(m_commandQueue, m_hwnd, &swapChainDesc, nullptr, nullptr, &swapChain));
		THROW_IF_FAILED(swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain)));

		m_swapChain->SetMaximumFrameLatency(s_frameCount);
		m_swapChainWaitableObject = m_swapChain->GetFrameLatencyWaitableObject();
	}

	CreateRenderTarget();
}

void Vacuum::DX12Renderer::LoadGUIShaders()
{
	static const char* vertexShaderStr =
		"cbuffer vertexBuffer : register(b0) \
        {\
            float4x4 ProjectionMatrix; \
        };\
        struct VS_INPUT\
        {\
            float2 pos : POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
        };\
        \
        struct PS_INPUT\
        {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
        };\
        \
        PS_INPUT vs_main(VS_INPUT input)\
        {\
            PS_INPUT output;\
            output.pos = mul( ProjectionMatrix, float4(input.pos.xy, 0.f, 1.f));\
            output.col = input.col;\
            output.uv  = input.uv;\
            return output;\
        }";

	static const char* pixelShaderStr =
		"struct PS_INPUT\
        {\
            float4 pos : SV_POSITION;\
            float4 col : COLOR0;\
            float2 uv  : TEXCOORD0;\
        };\
        SamplerState sampler0 : register(s0);\
        Texture2D texture0 : register(t0);\
        \
        float4 ps_main(PS_INPUT input) : SV_Target\
        {\
            float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
            return out_col; \
        }";

	THROW_IF_FAILED(D3DCompile(vertexShaderStr, strlen(vertexShaderStr), nullptr, nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &m_guiVertexShader, nullptr));
	THROW_IF_FAILED(D3DCompile(pixelShaderStr, strlen(pixelShaderStr), nullptr, nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &m_guiPixelShader, nullptr));
}

void Vacuum::DX12Renderer::LoadVPShaders()
{
	static const char* vertexShaderStr =
		"cbuffer vertexBuffer : register(b0) \
        {\
            float4x4 ProjectionMatrix; \
        };\
        struct VS_INPUT\
        {\
            float3 pos : POSITION;\
            float3 normal : NORMAL0;\
            float2 texcoord  : TEXCOORD0;\
        };\
        \
        struct PS_INPUT\
        {\
            float4 pos : SV_POSITION;\
        };\
        \
        PS_INPUT vs_main(VS_INPUT input)\
        {\
            PS_INPUT output;\
            output.pos = mul(ProjectionMatrix, float4(input.pos.xyz, 1.f));\
            return output;\
        }";

	static const char* pixelShaderStr =
		"struct PS_INPUT\
        {\
            float4 pos : SV_POSITION;\
        };\
        \
        float4 ps_main(PS_INPUT input) : SV_Target\
        {\
			float4 outCol = {1.0, 0.0, 0.0, 1.0};\
            return outCol; \
        }";

	THROW_IF_FAILED(D3DCompile(vertexShaderStr, strlen(vertexShaderStr), nullptr, nullptr, nullptr, "vs_main", "vs_5_0", 0, 0, &m_vpVertexShader, nullptr));
	THROW_IF_FAILED(D3DCompile(pixelShaderStr, strlen(pixelShaderStr), nullptr, nullptr, nullptr, "ps_main", "ps_5_0", 0, 0, &m_vpPixelShader, nullptr));

}

void Vacuum::DX12Renderer::LoadAssets()
{
	if (!m_guiVertexShader || !m_guiPixelShader)
	{
		LoadGUIShaders();
	}

	ComPtr<ID3DBlob> error;
	{
		D3D12_DESCRIPTOR_RANGE descRange = {};
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.NumDescriptors = 1;
		descRange.BaseShaderRegister = 0;
		descRange.RegisterSpace = 0;
		descRange.OffsetInDescriptorsFromTableStart = 0;

		D3D12_ROOT_PARAMETER param[2] = {};

		param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		param[0].Constants.ShaderRegister = 0;
		param[0].Constants.RegisterSpace = 0;
		param[0].Constants.Num32BitValues = 16;
		param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param[1].DescriptorTable.NumDescriptorRanges = 1;
		param[1].DescriptorTable.pDescriptorRanges = &descRange;
		param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC staticSampler = {};
		staticSampler.Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSampler.MipLODBias = 0.0f;
		staticSampler.MaxAnisotropy = 0;
		staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		staticSampler.MinLOD = 0.0f;
		staticSampler.MaxLOD = 0.0f;
		staticSampler.ShaderRegister = 0;
		staticSampler.RegisterSpace = 0;
		staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC desc = {};
		desc.NumParameters = _countof(param);
		desc.pParameters = param;
		desc.NumStaticSamplers = 1;
		desc.pStaticSamplers = &staticSampler;
		desc.Flags = 
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		ComPtr<ID3DBlob> blob = nullptr;
		THROW_IF_FAILED(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error));

		m_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_guiRootSignature));
	}

	{
#if defined(_DEBUG)
		u32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		uint32 compileFlags = 0;
#endif
		

		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (u32)DX_OFFSETOF(S2DVert, Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (u32)DX_OFFSETOF(S2DVert, UV),  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (u32)DX_OFFSETOF(S2DVert, Col), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, 3 };
		psoDesc.pRootSignature = m_guiRootSignature;
		psoDesc.NodeMask = 1;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_guiVertexShader);
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_guiPixelShader);
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

			// Create the blending setup
		{
			D3D12_BLEND_DESC& desc = psoDesc.BlendState;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}

			// Create the rasterizer state
		{
			D3D12_RASTERIZER_DESC& desc = psoDesc.RasterizerState;
			desc.FillMode = D3D12_FILL_MODE_SOLID;
			desc.CullMode = D3D12_CULL_MODE_NONE;
			desc.FrontCounterClockwise = FALSE;
			desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			desc.DepthClipEnable = true;
			desc.MultisampleEnable = FALSE;
			desc.AntialiasedLineEnable = FALSE;
			desc.ForcedSampleCount = 0;
			desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		}

			// Create depth-stencil State
		{
			D3D12_DEPTH_STENCIL_DESC& desc = psoDesc.DepthStencilState;
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			desc.BackFace = desc.FrontFace;

			THROW_IF_FAILED(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_guiPipelineState)));
		}
	}
}

void Vacuum::DX12Renderer::LoadVPAssets()
{
	if (!m_vpVertexShader || !m_vpPixelShader)
	{
		LoadVPShaders();
	}

	ComPtr<ID3DBlob> error;
	{
		D3D12_DESCRIPTOR_RANGE descRange = {};
		descRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descRange.NumDescriptors = 1;
		descRange.BaseShaderRegister = 0;
		descRange.RegisterSpace = 0;
		descRange.OffsetInDescriptorsFromTableStart = 0;

		D3D12_ROOT_PARAMETER param[2] = {};

		param[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
		param[0].Constants.ShaderRegister = 0;
		param[0].Constants.RegisterSpace = 0;
		param[0].Constants.Num32BitValues = 16;
		param[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

		param[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
		param[1].DescriptorTable.NumDescriptorRanges = 1;
		param[1].DescriptorTable.pDescriptorRanges = &descRange;
		param[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_STATIC_SAMPLER_DESC staticSampler = {};
		staticSampler.Filter = D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		staticSampler.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSampler.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSampler.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		staticSampler.MipLODBias = 0.0f;
		staticSampler.MaxAnisotropy = 0;
		staticSampler.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
		staticSampler.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		staticSampler.MinLOD = 0.0f;
		staticSampler.MaxLOD = 0.0f;
		staticSampler.ShaderRegister = 0;
		staticSampler.RegisterSpace = 0;
		staticSampler.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		D3D12_ROOT_SIGNATURE_DESC desc = {};
		desc.NumParameters = _countof(param);
		desc.pParameters = param;
		desc.NumStaticSamplers = 1;
		desc.pStaticSamplers = &staticSampler;
		desc.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
			D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

		ComPtr<ID3DBlob> blob = nullptr;
		THROW_IF_FAILED(D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob, &error));

		m_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_vpRootSignature));
	}

	{
#if defined(_DEBUG)
		u32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		uint32 compileFlags = 0;
#endif


		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0, (u32)DX_OFFSETOF(SVertex, Position), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0, (u32)DX_OFFSETOF(SVertex, Normal),  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT, 0, (u32)DX_OFFSETOF(SVertex, TextureCoordinate), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, 3 };
		psoDesc.pRootSignature = m_vpRootSignature;
		psoDesc.NodeMask = 1;
		psoDesc.VS = CD3DX12_SHADER_BYTECODE(m_vpVertexShader);
		psoDesc.PS = CD3DX12_SHADER_BYTECODE(m_vpPixelShader);
		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
		psoDesc.SampleMask = UINT_MAX;
		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		psoDesc.NumRenderTargets = 1;
		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
		psoDesc.SampleDesc.Count = 1;
		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;

		// Create the blending setup
		{
			D3D12_BLEND_DESC& desc = psoDesc.BlendState;
			desc.AlphaToCoverageEnable = false;
			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_INV_SRC_ALPHA;
			desc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
		}

		// Create the rasterizer state
		{
			D3D12_RASTERIZER_DESC& desc = psoDesc.RasterizerState;
			desc.FillMode = D3D12_FILL_MODE_SOLID;
			desc.CullMode = D3D12_CULL_MODE_NONE;
			desc.FrontCounterClockwise = FALSE;
			desc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
			desc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
			desc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
			desc.DepthClipEnable = true;
			desc.MultisampleEnable = FALSE;
			desc.AntialiasedLineEnable = FALSE;
			desc.ForcedSampleCount = 0;
			desc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
		}

		// Create depth-stencil State
		{
			D3D12_DEPTH_STENCIL_DESC& desc = psoDesc.DepthStencilState;
			desc.DepthEnable = false;
			desc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
			desc.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			desc.StencilEnable = false;
			desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
			desc.BackFace = desc.FrontFace;

			THROW_IF_FAILED(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_vpPipelineState)));
		}
	}
}

void Vacuum::DX12Renderer::GetHandwareAdapter(IDXGIFactory1* _factory, Microsoft::WRL::ComPtr<IDXGIAdapter1>& _adapter)
{
	_adapter = nullptr;

	ComPtr<IDXGIFactory6> factory;
	ComPtr<IDXGIAdapter1> tempHardwareAdapter;
	if (SUCCEEDED(_factory->QueryInterface(IID_PPV_ARGS(&factory))))
	{
		for (s32 adapterIndex = 0;
			DXGI_ERROR_NOT_FOUND != factory->EnumAdapterByGpuPreference(adapterIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tempHardwareAdapter));
			++adapterIndex)
		{
			DXGI_ADAPTER_DESC1 desc;
			tempHardwareAdapter->GetDesc1(&desc);

			if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			if (SUCCEEDED(D3D12CreateDevice(tempHardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
			{
				break;
			}
		}
	}
	_adapter = tempHardwareAdapter.Detach();
}

void Vacuum::DX12Renderer::CreateFontsTexture(unsigned char* _pixels, const s32& _width, const s32& _height, u64& _texID)
{
	// Upload texture to graphics system
	{
		D3D12_HEAP_PROPERTIES props;
		memset(&props, 0, sizeof(D3D12_HEAP_PROPERTIES));
		props.Type = D3D12_HEAP_TYPE_DEFAULT;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		desc.Alignment = 0;
		desc.Width = _width;
		desc.Height = _height;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		ID3D12Resource* pTexture = NULL;
		m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_COPY_DEST, NULL, IID_PPV_ARGS(&pTexture));

		UINT uploadPitch = (_width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
		UINT uploadSize = _height * uploadPitch;
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Alignment = 0;
		desc.Width = uploadSize;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		ID3D12Resource* uploadBuffer = NULL;
		HRESULT hr = m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc,
			D3D12_RESOURCE_STATE_GENERIC_READ, NULL, IID_PPV_ARGS(&uploadBuffer));
		THROW_IF_FAILED(hr);

		void* mapped = NULL;
		D3D12_RANGE range = { 0, uploadSize };
		hr = uploadBuffer->Map(0, &range, &mapped);
		THROW_IF_FAILED(hr);
		for (int y = 0; y < _height; y++)
			memcpy((void*)((uintptr_t)mapped + y * uploadPitch), _pixels + y * _width * 4, _width * 4);
		uploadBuffer->Unmap(0, &range);

		D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
		srcLocation.pResource = uploadBuffer;
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srcLocation.PlacedFootprint.Footprint.Width = _width;
		srcLocation.PlacedFootprint.Footprint.Height = _height;
		srcLocation.PlacedFootprint.Footprint.Depth = 1;
		srcLocation.PlacedFootprint.Footprint.RowPitch = uploadPitch;

		D3D12_TEXTURE_COPY_LOCATION dstLocation = {};
		dstLocation.pResource = pTexture;
		dstLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
		dstLocation.SubresourceIndex = 0;

		D3D12_RESOURCE_BARRIER barrier = {};
		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = pTexture;
		barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

		ID3D12Fence* fence = NULL;
		hr = m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		THROW_IF_FAILED(hr);

		HANDLE event = CreateEvent(0, 0, 0, 0);
		if (!event)
		{
			throw std::runtime_error("event handle invalid");
		}

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 1;

		ID3D12CommandQueue* cmdQueue = NULL;
		hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
		THROW_IF_FAILED(hr);

		ID3D12CommandAllocator* cmdAlloc = NULL;
		hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
		THROW_IF_FAILED(hr);

		ID3D12GraphicsCommandList* cmdList = NULL;
		hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
		THROW_IF_FAILED(hr);

		cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
		cmdList->ResourceBarrier(1, &barrier);

		hr = cmdList->Close();
		THROW_IF_FAILED(hr);

		cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
		hr = cmdQueue->Signal(fence, 1);
		THROW_IF_FAILED(hr);

		fence->SetEventOnCompletion(1, event);
		WaitForSingleObject(event, INFINITE);

		cmdList->Release();
		cmdAlloc->Release();
		cmdQueue->Release();
		CloseHandle(event);
		fence->Release();
		uploadBuffer->Release();

		// Create texture view
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));
		srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = desc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		m_device->CreateShaderResourceView(pTexture, &srvDesc, m_srvDescHeap->GetCPUDescriptorHandleForHeapStart());
		SafeRelease(m_fontTextureResource);
		m_fontTextureResource = pTexture;
	}

	// Store our identifier
	static_assert(sizeof(void*) >= sizeof(m_srvDescHeap->GetGPUDescriptorHandleForHeapStart().ptr), "Can't pack descriptor handle into TexID, 32-bit not supported yet.");
	_texID = m_srvDescHeap->GetGPUDescriptorHandleForHeapStart().ptr;
}

Vacuum::SFrameContext* Vacuum::DX12Renderer::WaitForNextFrameResources()
{
	u32 nextFrameIndex = m_frameIndex +1;
	m_frameIndex = nextFrameIndex;

	HANDLE waitableObjects[] = { m_swapChainWaitableObject, nullptr };
	DWORD numWaitableObjects = 1;

	SFrameContext* frameCtx = &m_frameContext[nextFrameIndex % s_frameCount];
	
	if (frameCtx->FenceValue!= 0)
	{
		frameCtx->FenceValue = 0;
		m_fence->SetEventOnCompletion(frameCtx->FenceValue, m_fenceEvent);
		waitableObjects[1] = m_fenceEvent;
		numWaitableObjects = 2;
	}

	WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

	return frameCtx;
}

void Vacuum::DX12Renderer::WaitForLastSubmittedFrame()
{
	SFrameContext* frameCtx = &m_frameContext[m_frameIndex % s_frameCount];

	u64 fenceValue = frameCtx->FenceValue;
	if (fenceValue == 0)
	{
		return;
	}

	frameCtx->FenceValue = 0;
	if (m_fence->GetCompletedValue() < fenceValue)
	{
		m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}
}

void Vacuum::DX12Renderer::SetupRenderState(SGuiDrawData* _drawData, SFrameResource* _frameResource)
{
	SVertexConstantBuffer vertConstBuffer;
	{
		float l = _drawData->DisplayPos.x;
		float r = _drawData->DisplayPos.x + _drawData->DisplaySize.x;
		float t = _drawData->DisplayPos.y;
		float b = _drawData->DisplayPos.y + _drawData->DisplaySize.y;
		float mvp[4][4] =
		{
			{2.0f/(r-l),		0.0f,			0.0f,			0.0f},
			{0.0f,				2.0f/(t-b),		0.0f,			0.0f},
			{0.0f,				0.0f,			0.5f,			0.0f},
			{(r+l)/(l-r),		(t+b)/(b-t),	0.5,			1.0f},
		};
		memcpy(&vertConstBuffer.MVP, mvp, sizeof(mvp));
	}

	D3D12_VIEWPORT viewPort = {};
	viewPort.Width = _drawData->DisplaySize.x;
	viewPort.Height = _drawData->DisplaySize.y;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;

	m_guiCommandList->RSSetViewports(1, &viewPort); //this needs more viewports since I want to make my editor viewport work here

	u32 stride = sizeof(S2DVert);
	u32 offset = 0;

	D3D12_VERTEX_BUFFER_VIEW vtxBufferView = {};
	vtxBufferView.BufferLocation = _frameResource->VertexBuffer->GetGPUVirtualAddress() + offset;
	vtxBufferView.SizeInBytes = _frameResource->VertexBufferSize * stride;
	vtxBufferView.StrideInBytes = stride;

	m_guiCommandList->IASetVertexBuffers(0, 1, &vtxBufferView); //This needs to get reworked to make the editor viewport work

	D3D12_INDEX_BUFFER_VIEW idxBufferView = {};
	idxBufferView.BufferLocation = _frameResource->IndexBuffer->GetGPUVirtualAddress();
	idxBufferView.SizeInBytes = _frameResource->IndexBufferSize * sizeof(u16);
	idxBufferView.Format = DXGI_FORMAT_R16_UINT;

	m_guiCommandList->IASetIndexBuffer(&idxBufferView);
	m_guiCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_guiCommandList->SetPipelineState(m_guiPipelineState);
	m_guiCommandList->SetGraphicsRootSignature(m_guiRootSignature);
	m_guiCommandList->SetGraphicsRoot32BitConstants(0, 16, &vertConstBuffer, 0);

	const float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_guiCommandList->OMSetBlendFactor(blendFactor);
}

void Vacuum::DX12Renderer::SetupViewportRenderState(SDrawData* _drawData, SFrameResource* _frameResource)
{
	SVertexConstantBuffer vertConstBuffer;
	{
		float l = _drawData->DisplayPos.x; // add here the x pos of the viewport 
		float r = _drawData->DisplayPos.x + _drawData->DisplaySize.x; // add here the width of the viewport
		float t = _drawData->DisplayPos.y; // add here the y pos of the viewport
		float b = _drawData->DisplayPos.y + _drawData->DisplaySize.y; // add here the height of the viewport
		float mvp[4][4] = //This needs to get reworked to make a real mvp for a camera
		{
			{2.0f / (r - l),		0.0f,			0.0f,			0.0f},
			{0.0f,				2.0f / (t - b),		0.0f,			0.0f},
			{0.0f,				0.0f,			0.5f,			0.0f},
			{(r + l) / (l - r),		(t + b) / (b - t),	0.5,			1.0f},
		};
		memcpy(&vertConstBuffer.MVP, mvp, sizeof(mvp));
	}

	D3D12_VIEWPORT viewPort = {};
	viewPort.Width = _drawData->DisplaySize.x; //this will need the actual width of the viewport
	viewPort.Height = _drawData->DisplaySize.y; //this will need the actual height of the viewport
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = _drawData->DisplayPos.x;
	viewPort.TopLeftY = _drawData->DisplayPos.y;

	m_vpCommandList->RSSetViewports(1, &viewPort);

	u32 stride = sizeof(SVertex);
	u32 offset = 0;

	D3D12_VERTEX_BUFFER_VIEW vtxBufferView = {};
	vtxBufferView.BufferLocation = _frameResource->VertexBuffer->GetGPUVirtualAddress() + offset; // The bufferlocation of the vertexbuffer for the viewport
	vtxBufferView.SizeInBytes = _frameResource->VertexBufferSize * stride; //The size of the vertexbuffer 
	vtxBufferView.StrideInBytes = stride;

	m_vpCommandList->IASetVertexBuffers(0, 1, &vtxBufferView);

	D3D12_INDEX_BUFFER_VIEW idxBufferView = {};
	idxBufferView.BufferLocation = _frameResource->IndexBuffer->GetGPUVirtualAddress(); //The actual index buffer location
	idxBufferView.SizeInBytes = _frameResource->IndexBufferSize * sizeof(u16); // The size of the index buffer
	idxBufferView.Format = DXGI_FORMAT_R16_UINT; //Idx buffer format

	m_vpCommandList->IASetIndexBuffer(&idxBufferView);
	m_vpCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_vpCommandList->SetPipelineState(m_vpPipelineState);
	m_vpCommandList->SetGraphicsRootSignature(m_vpRootSignature);
	m_vpCommandList->SetGraphicsRoot32BitConstants(0, 16, &vertConstBuffer, 0);

	const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_vpCommandList->OMSetBlendFactor(blendFactor);
}

void Vacuum::DX12Renderer::InvalidateObjects()
{
	if (!m_device)
	{
		return;
	}

	SafeRelease(m_guiRootSignature);
	SafeRelease(m_guiPipelineState);
	SafeRelease(m_fontTextureResource);

	for (u32 i = 0; i < s_frameCount; ++i)
	{
		SFrameResources* frameResources = &m_frameResources[i];

		SafeRelease(frameResources->GUIFrameResources->VertexBuffer);
		SafeRelease(frameResources->GUIFrameResources->IndexBuffer);
		SafeRelease(frameResources->VPFrameResources->VertexBuffer);
		SafeRelease(frameResources->VPFrameResources->IndexBuffer);
	}
}

void Vacuum::DX12Renderer::CreateRenderTarget()
{
	for (u32 i = 0; i < s_frameCount; ++i)
	{
		ID3D12Resource* backBuffer = nullptr;
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer));
		m_device->CreateRenderTargetView(backBuffer, nullptr, m_renderTargetDescs[i]);
		m_renderTargets[i] = backBuffer;
	}
}

void Vacuum::DX12Renderer::CleanupRenderTarget()
{
	WaitForLastSubmittedFrame();

	for (u32 i = 0; i < s_frameCount; ++i)
	{
		SafeRelease(m_renderTargets[i]);
	}
}

void Vacuum::DX12Renderer::ResizeSwapChain(HWND _hwnd, const u32& _width, const u32& _height)
{
	DXGI_SWAP_CHAIN_DESC1 desc = {};
	m_swapChain->GetDesc1(&desc);
	desc.Width = _width;
	desc.Height = _height;

	IDXGIFactory4* factory = nullptr;
	m_swapChain->GetParent(IID_PPV_ARGS(&factory));

	m_swapChain->Release();
	CloseHandle(m_swapChainWaitableObject);

	IDXGISwapChain1* swapChain = nullptr;
	factory->CreateSwapChainForHwnd(m_commandQueue, _hwnd, &desc, nullptr, nullptr, &swapChain);
	swapChain->QueryInterface(IID_PPV_ARGS(&m_swapChain));
	swapChain->Release();
	factory->Release();

	m_swapChain->SetMaximumFrameLatency(s_frameCount);

	m_swapChainWaitableObject = m_swapChain->GetFrameLatencyWaitableObject();
	assert(m_swapChainWaitableObject != nullptr);;
}
