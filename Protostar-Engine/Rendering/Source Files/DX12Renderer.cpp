#include "..\Header Files\Private\DX12Renderer.h"

#pragma region Internal Includes 
#include "..\Header Files\Private\DXHelper.h"
#include "Window.h"
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

void Protostar::DX12Renderer::OnCreate()
{
	CMainWindow* mainWindow = CMainWindow::GetWindowHandle();
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
				for (const std::function<void(HWND, u32, WPARAM, LPARAM)>& func : m_afterResizeCallbacks)
				{
					func(_hwnd, _msg, _wParam, _lParam);
				}
			}
			return 0;
		});

	mainWindow->RegisterCallbackForWMEvents(WM_SIZE, func);
}

void Protostar::DX12Renderer::OnInit()
{
	LoadAssets();
	LoadVPAssets();
}

void Protostar::DX12Renderer::OnUpdate()
{
	if (m_windowSize.x <= 0.0f || m_windowSize.y <= 0.0f)
	{
		return;
	}

	m_frameIndex = m_frameIndex + 1;

	SFrameResource* frameResource = &m_frameResources[m_frameIndex % s_frameCount];

	if (frameResource->VertexBuffer == nullptr || frameResource->VertexBufferSize < m_totalVtxSize)
	{
		SafeRelease(frameResource->VertexBuffer);
		frameResource->VertexBufferSize = m_totalVtxSize + 5000;

		//PE_LOG_F("Resize vertex buffer to: %i", m_totalVtxSize + 5000);

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = m_totalVtxSize;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResource->VertexBuffer)) < 0)
		{
			PE_LOG("Couldn't create vertex buffer");
			return;
		}
	}
	if (frameResource->IndexBuffer == nullptr || frameResource->IndexBufferSize < m_totalIdxSize)
	{
		SafeRelease(frameResource->IndexBuffer);
		frameResource->IndexBufferSize = m_totalIdxSize + 2000;
		//PE_LOG_F("Resize index buffer to: %i", m_totalIdxSize + 2000);

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = m_totalIdxSize;
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResource->IndexBuffer)) < 0)
		{
			PE_LOG("Couldn't create index buffer");
			return;
		}
	}

	void* vtxResource;
	void* idxResource;
	D3D12_RANGE range = {};

	if (frameResource->VertexBuffer->Map(0, &range, &vtxResource) != S_OK)
	{
		return;
	}
	if (frameResource->IndexBuffer->Map(0, &range, &idxResource) != S_OK)
	{
		return;
	}

	S2DVert* vtxDestination = (S2DVert*)vtxResource;
	u16* idxDestination = (u16*)idxResource;

	u32 currentVtxOffset = 0;
	u32 currentIdxOffset = 0;
	u32 idxStride = sizeof(u16);
	u32 vtx2DStride = sizeof(S2DVert);
	u32 vtx3DStride = sizeof(SVertex);

	for (const SGuiDrawList& drawList : m_guiDrawData->DrawLists)
	{
		m_currentVertexBufferSize += drawList.VertexBuffer.size() * vtx2DStride;
		m_currentIndexBufferSize += drawList.IndexBuffer.size() * idxStride;

		memcpy(vtxDestination, drawList.VertexBuffer.data(), drawList.VertexBuffer.size() * vtx2DStride);
		memcpy(idxDestination, drawList.IndexBuffer.data(), drawList.IndexBuffer.size() * idxStride);
		vtxDestination += drawList.VertexBuffer.size();
		idxDestination += drawList.IndexBuffer.size();

		currentVtxOffset += drawList.VertexBuffer.size() * vtx2DStride;
		currentIdxOffset += drawList.IndexBuffer.size() * idxStride;

		m_currentVertexBufferSize += drawList.VertexBuffer.size();
		m_currentIndexBufferSize += drawList.IndexBuffer.size();
	}

	SVertex* vtx3DDestination = (SVertex*)vtxDestination;

	for (SDrawData* drawData : m_drawDatas)
	{
		for(const SDrawList& drawList : drawData->DrawLists)
		{
			memcpy(vtx3DDestination, drawList.VertexBuffer.data(), drawList.VertexBuffer.size() * vtx3DStride);
			memcpy(idxDestination, drawList.IndexBuffer.data(), drawList.IndexBuffer.size() * idxStride);
			vtx3DDestination += drawList.VertexBuffer.size();
			idxDestination += drawList.IndexBuffer.size();
		}
	}

	frameResource->VertexBuffer->Unmap(0, &range);
	frameResource->IndexBuffer->Unmap(0, &range);

	SetupRenderState(m_guiDrawData, frameResource);

	DirectX::XMFLOAT2 clipOff = m_guiDrawData->DisplayPos;
	u32 idxOffset = 0;
	u32 vtxOffset = 0;
	for (const SGuiDrawList& drawList : m_guiDrawData->DrawLists)
	{
		for (const SGuiDrawCmd& drawCmd : drawList.DrawCommands)
		{
			if (drawCmd.UserCallbackValid)
			{
				if (!drawCmd.CallUserCallback)
				{
					SetupRenderState(m_guiDrawData, frameResource);
				}
				else
				{
					drawCmd.UserCallback();
				}
			}
			else
			{
				const D3D12_RECT rect = { (LONG)(drawCmd.ClipRect.x, -clipOff.x), (LONG)(drawCmd.ClipRect.y - clipOff.y), (LONG)(drawCmd.ClipRect.z - clipOff.x), (LONG)(drawCmd.ClipRect.w - clipOff.y) };
				m_commandList->SetGraphicsRootDescriptorTable(1, *(D3D12_GPU_DESCRIPTOR_HANDLE*)&drawCmd.TextureID);
				m_commandList->RSSetScissorRects(1, &rect);
				m_commandList->DrawIndexedInstanced(drawCmd.ElemCount, 1, drawCmd.IdxOffset + idxOffset, drawCmd.VtxOffset + vtxOffset, 0);
			}
		}

		vtxOffset += drawList.VertexBuffer.size();
		idxOffset += drawList.IndexBuffer.size();

		m_currentVtxBufferOffset += drawList.VertexBuffer.size() * sizeof(S2DVert);
		m_currentIdxBufferOffset += drawList.IndexBuffer.size() * sizeof(u16);
	}

	for (SDrawData* drawData : m_drawDatas)
	{
		SetupVPRenderState(drawData, frameResource);

		DirectX::XMFLOAT2 vpClipOff = drawData->DisplayPos;

		for (const SDrawList& drawList : drawData->DrawLists)
		{
			for (const SDrawCmd& drawCmd : drawList.DrawCmds)
			{
				const D3D12_RECT rect = { (LONG)(drawCmd.ClipRect.x, + vpClipOff.x), (LONG)(drawCmd.ClipRect.y + vpClipOff.y), (LONG)(drawCmd.ClipRect.z - vpClipOff.x), (LONG)(drawCmd.ClipRect.w - vpClipOff.y) };
				m_commandList->RSSetScissorRects(1, &rect);
				m_commandList->DrawIndexedInstanced(drawCmd.ElemCount, 1, 0, 0, 0);
			}

			m_currentVtxBufferOffset += drawList.VertexBuffer.size();
			m_currentIdxBufferOffset += drawList.IndexBuffer.size();
		}
		delete drawData;
		drawData = nullptr;
	}

	m_drawDatas.clear();

	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	m_commandList->ResourceBarrier(1, &m_barrier);
	m_commandList->Close();
}

void Protostar::DX12Renderer::PrepareRendering()
{
	if (!m_pipelineState)
	{
		LoadAssets();
	}

	SFrameContext* frameCtx = WaitForNextFrameResources();
	u32 backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
	frameCtx->CommandAllocator->Reset();

	m_barrier = {};
	m_barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	m_barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	m_barrier.Transition.pResource = m_renderTargets[backBufferIndex];
	m_barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	const float clearColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	THROW_IF_FAILED(m_commandList->Reset(frameCtx->CommandAllocator, nullptr));
	m_commandList->ResourceBarrier(1, &m_barrier);
	m_commandList->ClearRenderTargetView(m_renderTargetDescs[backBufferIndex], clearColor, 0, nullptr);
	m_commandList->OMSetRenderTargets(1, &m_renderTargetDescs[backBufferIndex], FALSE, nullptr);
	m_commandList->SetDescriptorHeaps(1, &m_srvDescHeap);
}

void Protostar::DX12Renderer::UpdateGuiDrawData(SGuiDrawData* _drawData)
{
	m_windowSize = _drawData->DisplaySize;
	
	m_guiDrawData = _drawData;

	m_totalIdxCount += _drawData->TotalIdxCount;
	m_totalVtxCount += _drawData->TotalVtxCount;
	m_totalVtxSize += _drawData->TotalVtxCount * sizeof(S2DVert);
	m_totalIdxSize += _drawData->TotalIdxCount * sizeof(u16);
	m_totalIdxSize += _drawData->TotalIdxCount;
	m_totalVtxSize += _drawData->TotalVtxCount;
}

void Protostar::DX12Renderer::AddDrawData(SDrawData* _drawData)
{
	m_drawDatas.push_back(_drawData);

	m_totalIdxCount += _drawData->TotalIdxCount;
	m_totalVtxCount += _drawData->TotalVtxCount;

	m_totalIdxSize += _drawData->TotalIdxCount * sizeof(u16);
	m_totalVtxSize += _drawData->TotalVtxCount * sizeof(SVertex);
	m_totalIdxSize += _drawData->TotalIdxCount;
	m_totalVtxSize += _drawData->TotalVtxCount;
}

void Protostar::DX12Renderer::OnRender()
{
	std::vector<ID3D12CommandList*> commandLists = std::vector<ID3D12CommandList*>(1);
	commandLists[0] = m_commandList;

	m_commandQueue->ExecuteCommandLists(1, commandLists.data());
	m_swapChain->Present(m_bVSync, 0);

	u64 fenceValue = m_fenceValue + 1;
	m_commandQueue->Signal(m_fence, fenceValue);
	m_fenceValue = fenceValue;

	SFrameContext* frameCtx = &m_frameContext[m_frameIndex % s_frameCount];
	frameCtx->FenceValue = fenceValue;

	m_currentVertexBufferSize = 0;
	m_currentIndexBufferSize = 0;
	m_currentVtxBufferOffset = 0;
	m_currentIdxBufferOffset = 0;

	m_totalIdxCount = 0;
	m_totalVtxCount = 0;
	m_totalIdxSize = 0;
	m_totalVtxSize = 0;
}

void Protostar::DX12Renderer::OnDestroy()
{
	InvalidateObjects();

	delete[] m_frameResources;
	m_frameResources = nullptr;
	m_device = nullptr;
	m_frameIndex = UINT_MAX;

	CleanupRenderTarget();
	SafeRelease(m_swapChain);
	if (m_swapChainWaitableObject) CloseHandle(m_swapChainWaitableObject);
	for (u32 i = 0; i < s_frameCount; ++i)
	{
		SafeRelease(m_frameContext[i].CommandAllocator);
	}
	SafeRelease(m_commandQueue);
	SafeRelease(m_commandList);
	SafeRelease(m_srvDescHeap);
	SafeRelease(m_rtvHeap);
	SafeRelease(m_fence);
	SafeRelease(m_guiVertexShader);
	SafeRelease(m_guiPixelShader);
	if (m_fenceEvent) CloseHandle(m_fenceEvent); m_fenceEvent = nullptr;
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

void Protostar::DX12Renderer::RegisterAfterResizeCallback(const std::function<void(HWND, u32, WPARAM, LPARAM)>& _func)
{
	m_afterResizeCallbacks.push_back(_func);
}

bool Protostar::DX12Renderer::CreateRootSignature(ID3DBlob* _rootSignatureBlob, ID3D12RootSignature** _rootSignature)
{
	HRESULT hr = m_device->CreateRootSignature(0, _rootSignatureBlob->GetBufferPointer(), _rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(_rootSignature));
	if (hr != S_OK)
	{
		PE_LOG(HrToString(hr).c_str());
		return false;
	}
	return true;
}

bool Protostar::DX12Renderer::CreatePSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC* _desc, ID3D12PipelineState** _pso)
{
	HRESULT hr = m_device->CreateGraphicsPipelineState(_desc, IID_PPV_ARGS(_pso));
	if (hr != S_OK)
	{
		PE_LOG(HrToString(hr).c_str());
		return false;
	}
	return true;
}

void Protostar::DX12Renderer::LoadPipeline()
{
#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	m_frameResources = new SFrameResource[s_frameCount];

	for (s32 i = 0; i < s_frameCount; ++i)
	{
		SFrameResource* frameResource = &m_frameResources[i];
		frameResource->IndexBuffer = nullptr;
		frameResource->VertexBuffer = nullptr;
		frameResource->IndexBufferSize = 10000;
		frameResource->VertexBufferSize = 5000;
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
		D3D12_COMMAND_QUEUE_DESC desc = {};
		desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		desc.NodeMask = 1;
		THROW_IF_FAILED(m_device->CreateCommandQueue(&desc, IID_PPV_ARGS(&m_commandQueue)));
	}

	for (s32 i = 0; i < s_frameCount; ++i)
	{
		THROW_IF_FAILED(m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_frameContext[i].CommandAllocator)));
	}

	THROW_IF_FAILED(m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_frameContext[0].CommandAllocator, nullptr, IID_PPV_ARGS(&m_commandList)));

	THROW_IF_FAILED(m_commandList->Close());

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

void Protostar::DX12Renderer::LoadGUIShaders()
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

void Protostar::DX12Renderer::LoadVPShaders()
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

void Protostar::DX12Renderer::LoadAssets()
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

		m_device->CreateRootSignature(0, blob->GetBufferPointer(), blob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	}

	{
#if defined(_DEBUG)
		u32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		u32 compileFlags = 0;
#endif


		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (u32)DX_OFFSETOF(S2DVert, Pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (u32)DX_OFFSETOF(S2DVert, UV),  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (u32)DX_OFFSETOF(S2DVert, Col), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, 3 };
		psoDesc.pRootSignature = m_rootSignature;
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

			THROW_IF_FAILED(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
		}
	}
}

void Protostar::DX12Renderer::LoadVPAssets()
{
	if (!m_vpVertexShader || !m_vpPixelShader)
	{
		LoadVPShaders();
	}

	{
#if defined(_DEBUG)
		u32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		u32 compileFlags = 0;
#endif


		static D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,   0, (u32)DX_OFFSETOF(SVertex, Position), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT, 0, (u32)DX_OFFSETOF(SVertex, TextureCoordinate), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (u32)DX_OFFSETOF(SVertex, Normal),  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		};

		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
		psoDesc.InputLayout = { inputElementDescs, 3 };
		psoDesc.pRootSignature = m_rootSignature;
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

void Protostar::DX12Renderer::GetHandwareAdapter(IDXGIFactory1* _factory, Microsoft::WRL::ComPtr<IDXGIAdapter1>& _adapter)
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

void Protostar::DX12Renderer::CreateFontsTexture(unsigned char* _pixels, const s32& _width, const s32& _height, u64& _texID)
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

Protostar::SFrameContext* Protostar::DX12Renderer::WaitForNextFrameResources()
{
	u32 nextFrameIndex = m_frameIndex + 1;
	m_frameIndex = nextFrameIndex;

	HANDLE waitableObjects[] = { m_swapChainWaitableObject, nullptr };
	DWORD numWaitableObjects = 1;

	SFrameContext* frameCtx = &m_frameContext[nextFrameIndex % s_frameCount];
	u64 fenceValue = frameCtx->FenceValue;
	if (fenceValue != 0)
	{
		frameCtx->FenceValue = 0;
		m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
		waitableObjects[1] = m_fenceEvent;
		numWaitableObjects = 2;
	}

	WaitForMultipleObjects(numWaitableObjects, waitableObjects, TRUE, INFINITE);

	return frameCtx;
}

void Protostar::DX12Renderer::WaitForLastSubmittedFrame()
{
	SFrameContext* frameCtx = &m_frameContext[m_frameIndex % s_frameCount];

	u64 fenceValue = frameCtx->FenceValue;
	if (fenceValue == 0)
	{
		return;
	}

	frameCtx->FenceValue = 0;
	if (m_fence->GetCompletedValue() >= fenceValue)
	{
		return;
	}

	m_fence->SetEventOnCompletion(fenceValue, m_fenceEvent);
	WaitForSingleObject(m_fenceEvent, INFINITE);
}

void Protostar::DX12Renderer::SetupRenderState(SGuiDrawData* _drawData, SFrameResource* _frameResource)
{
	SVertexConstantBuffer vertConstBuffer;
	{
		float l = _drawData->DisplayPos.x;
		float r = _drawData->DisplayPos.x + _drawData->DisplaySize.x;
		float t = _drawData->DisplayPos.y;
		float b = _drawData->DisplayPos.y + _drawData->DisplaySize.y;
		float mvp[4][4] =
		{
			{2.0f / (r - l),		0.0f,			0.0f,			0.0f},
			{0.0f,				2.0f / (t - b),		0.0f,			0.0f},
			{0.0f,				0.0f,			0.5f,			0.0f},
			{(r + l) / (l - r),		(t + b) / (b - t),	0.5,			1.0f},
		};
		memcpy(&vertConstBuffer.MVP, mvp, sizeof(mvp));
	}

	D3D12_VIEWPORT viewPort = {};
	viewPort.Width = _drawData->DisplaySize.x;
	viewPort.Height = _drawData->DisplaySize.y;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = viewPort.TopLeftY = 0.0f;

	m_commandList->RSSetViewports(1, &viewPort); //this needs more viewports since I want to make my editor viewport work here

	u32 stride = sizeof(S2DVert);
	u32 offset = 0;

	D3D12_VERTEX_BUFFER_VIEW vtxBufferView = {};
	vtxBufferView.BufferLocation = _frameResource->VertexBuffer->GetGPUVirtualAddress() + offset;
	vtxBufferView.SizeInBytes = _drawData->TotalVtxCount * stride;
	vtxBufferView.StrideInBytes = stride;

	m_commandList->IASetVertexBuffers(0, 1, &vtxBufferView); //This needs to get reworked to make the editor viewport work

	D3D12_INDEX_BUFFER_VIEW idxBufferView = {};
	idxBufferView.BufferLocation = _frameResource->IndexBuffer->GetGPUVirtualAddress();
	idxBufferView.SizeInBytes = _drawData->TotalIdxCount * sizeof(u16);
	idxBufferView.Format = DXGI_FORMAT_R16_UINT;

	m_commandList->IASetIndexBuffer(&idxBufferView);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->SetPipelineState(m_pipelineState);
	m_commandList->SetGraphicsRootSignature(m_rootSignature);
	m_commandList->SetGraphicsRoot32BitConstants(0, 16, &vertConstBuffer, 0);

	const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_commandList->OMSetBlendFactor(blendFactor);
}

void Protostar::DX12Renderer::SetupVPRenderState(SDrawData* _drawData, SFrameResource* _frameResource)
{
	SVertexConstantBuffer vertConstBuffer;
	{
		float l = _drawData->DisplayPos.x;
		float r = _drawData->DisplayPos.x + _drawData->DisplaySize.x;
		float t = _drawData->DisplayPos.y;
		float b = _drawData->DisplayPos.y + _drawData->DisplaySize.y;
		float mvp[4][4] =
		{
			{1,		0.0f,			0.0f,			0.0f},
			{0.0f,				1,		0.0f,			0.0f},
			{0.0f,				0.0f,			1,			0.0f},
			{0,		0,	0,			1.0f},
		};
		memcpy(&vertConstBuffer.MVP, mvp, sizeof(mvp));
	}

	D3D12_VIEWPORT viewPort = {};
	viewPort.Width = _drawData->DisplaySize.x;
	viewPort.Height = _drawData->DisplaySize.y;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = _drawData->DisplayPos.x;
	viewPort.TopLeftY = _drawData->DisplayPos.y;

	m_commandList->RSSetViewports(1, &viewPort); //this needs more viewports since I want to make my editor viewport work here

	u32 stride = sizeof(SVertex);

	D3D12_VERTEX_BUFFER_VIEW vtxBufferView = {};
	vtxBufferView.BufferLocation = _frameResource->VertexBuffer->GetGPUVirtualAddress() + m_currentVtxBufferOffset;
	vtxBufferView.SizeInBytes = _drawData->DrawLists[0].VertexBuffer.size() * stride;
	vtxBufferView.StrideInBytes = stride;

	m_commandList->IASetVertexBuffers(0, 1, &vtxBufferView); //This needs to get reworked to make the editor viewport work

	D3D12_INDEX_BUFFER_VIEW idxBufferView = {};
	idxBufferView.BufferLocation = _frameResource->IndexBuffer->GetGPUVirtualAddress() + m_currentIdxBufferOffset;
	idxBufferView.SizeInBytes = _drawData->DrawLists[0].IndexBuffer.size() * sizeof(u16);
	idxBufferView.Format = DXGI_FORMAT_R16_UINT;

	m_commandList->IASetIndexBuffer(&idxBufferView);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->SetPipelineState(m_vpPipelineState);
	m_commandList->SetGraphicsRootSignature(m_rootSignature);
	m_commandList->SetGraphicsRoot32BitConstants(0, 16, &vertConstBuffer, 0);

	const float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_commandList->OMSetBlendFactor(blendFactor);
}

void Protostar::DX12Renderer::InvalidateObjects()
{
	if (!m_device)
	{
		return;
	}

	SafeRelease(m_rootSignature);
	SafeRelease(m_pipelineState);
	SafeRelease(m_fontTextureResource);

	for (u32 i = 0; i < s_frameCount; ++i)
	{
		SFrameResource* frameResource = &m_frameResources[i];

		SafeRelease(frameResource->VertexBuffer);
		SafeRelease(frameResource->IndexBuffer);
	}
}

void Protostar::DX12Renderer::CreateRenderTarget()
{
	for (u32 i = 0; i < s_frameCount; ++i)
	{
		ID3D12Resource* backBuffer = nullptr;
		m_swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer));
		m_device->CreateRenderTargetView(backBuffer, nullptr, m_renderTargetDescs[i]);
		m_renderTargets[i] = backBuffer;
	}
}

void Protostar::DX12Renderer::CleanupRenderTarget()
{
	WaitForLastSubmittedFrame();

	for (u32 i = 0; i < s_frameCount; ++i)
	{
		SafeRelease(m_renderTargets[i]);
	}
}

void Protostar::DX12Renderer::ResizeSwapChain(HWND _hwnd, const u32& _width, const u32& _height)
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
