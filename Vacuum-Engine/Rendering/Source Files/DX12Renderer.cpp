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

void Vacuum::DX12Renderer::OnCreate()
{
	CMainWindow* mainWindow =  CMainWindow::GetWindowHandle();
	if (!mainWindow)
	{
		return;
	}
	LoadPipeline();

	std::function<int32(HWND, uint32, WPARAM, LPARAM)> func([this](HWND _hwnd, uint32 _msg, WPARAM _wParam, LPARAM _lParam)->int32
	{
			if (m_device != nullptr && _wParam != SIZE_MINIMIZED)
			{
				WaitForLastSubmittedFrame();
				InvalidateObjects();
				CleanupRenderTarget();
				ResizeSwapChain(_hwnd, (uint32)LOWORD(_lParam), (uint32)HIWORD(_lParam));
				CreateRenderTarget();
				LoadAssets();
				CreateFontsTexture();
			}
			return 0;
	});

	mainWindow->RegisterCallbackForWMEvents(WM_SIZE, func);
}

void Vacuum::DX12Renderer::OnInit()
{
	LoadAssets();
	CreateFontsTexture();

}

void Vacuum::DX12Renderer::OnUpdate()
{

}

void Vacuum::DX12Renderer::PrepareRendering()
{
	if (!m_pipelineState)
	{
		LoadAssets();
	}

	SFrameContext* frameCtx = WaitForNextFrameResources();
	uint32 backBufferIndex = m_swapChain->GetCurrentBackBufferIndex();
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

void Vacuum::DX12Renderer::OnRender()
{
	ImDrawData* drawData = ImGui::GetDrawData();
	if (drawData->DisplaySize.x <= 0.0f || drawData->DisplaySize.y <= 0.0f)
	{
		return;
	} 

	m_frameIndex = m_frameIndex + 1;
	SFrameResource* frameResource = &m_frameResources[m_frameIndex % s_frameCount];

	if (frameResource->VertexBuffer == nullptr || frameResource->VertexBufferSize < drawData->TotalVtxCount)
	{
		SafeRelease(frameResource->VertexBuffer);
		frameResource->VertexBufferSize = drawData->TotalVtxCount + 5000;

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = frameResource->VertexBufferSize * sizeof(ImDrawVert);
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResource->VertexBuffer)) < 0)
		{
			return;
		}
	}
	if(frameResource->IndexBuffer == nullptr || frameResource->IndexBufferSize < drawData->TotalIdxCount)
	{
		SafeRelease(frameResource->IndexBuffer);
		frameResource->IndexBufferSize = drawData->TotalIdxCount + 10000;

		D3D12_HEAP_PROPERTIES props = {};
		props.Type = D3D12_HEAP_TYPE_UPLOAD;
		props.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		props.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;

		D3D12_RESOURCE_DESC desc = {};
		desc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		desc.Width = frameResource->IndexBufferSize * sizeof(ImDrawIdx);
		desc.Height = 1;
		desc.DepthOrArraySize = 1;
		desc.MipLevels = 1;
		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.SampleDesc.Count = 1;
		desc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		desc.Flags = D3D12_RESOURCE_FLAG_NONE;

		if (m_device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&frameResource->IndexBuffer)) < 0)
		{
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

	ImDrawVert* vtxDestination = (ImDrawVert*)vtxResource;
	ImDrawIdx* idxDestination = (ImDrawIdx*)idxResource;

	for (int32 i = 0; i < drawData->CmdListsCount; ++i)
	{
		const ImDrawList* cmdList = drawData->CmdLists[i];
		memcpy(vtxDestination, cmdList->VtxBuffer.Data, cmdList->VtxBuffer.Size * sizeof(ImDrawVert));
		memcpy(idxDestination, cmdList->IdxBuffer.Data, cmdList->IdxBuffer.Size * sizeof(ImDrawIdx));
		vtxDestination += cmdList->VtxBuffer.Size;
		idxDestination += cmdList->IdxBuffer.Size;
	}
	frameResource->VertexBuffer->Unmap(0, &range);
	frameResource->IndexBuffer->Unmap(0, &range);

	SetupRenderState(drawData, frameResource);

	int32 vtxOffset = 0;
	int32 idxOffset = 0;
	ImVec2 clipOff = drawData->DisplayPos;
	for (int32 i = 0; i < drawData->CmdListsCount; ++i)
	{
		const ImDrawList* cmdList = drawData->CmdLists[i];
		for (int32 cmdIndex = 0; cmdIndex < cmdList->CmdBuffer.Size; ++cmdIndex)
		{
			const ImDrawCmd* cmd = &cmdList->CmdBuffer[cmdIndex];
			if (cmd->UserCallback != nullptr)
			{
				if (cmd->UserCallback == ImDrawCallback_ResetRenderState)
				{
					SetupRenderState(drawData, frameResource);
				}
				else
				{
					cmd->UserCallback(cmdList, cmd);
				}
			}
			else
			{
				const D3D12_RECT rect = {(LONG)(cmd->ClipRect.x, - clipOff.x), (LONG)(cmd->ClipRect.y - clipOff.y), (LONG)(cmd->ClipRect.z - clipOff.x), (LONG)(cmd->ClipRect.w - clipOff.y)};
				m_commandList->SetGraphicsRootDescriptorTable(1, *(D3D12_GPU_DESCRIPTOR_HANDLE*)&cmd->TextureId);
				m_commandList->RSSetScissorRects(1, &rect);
				m_commandList->DrawIndexedInstanced(cmd->ElemCount, 1, cmd->IdxOffset + idxOffset, cmd->VtxOffset + vtxOffset, 0);
			}
		}
		idxOffset += cmdList->IdxBuffer.Size;
		vtxOffset += cmdList->VtxBuffer.Size;
	}
	m_barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	m_barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;

	m_commandList->ResourceBarrier(1, &m_barrier);
	m_commandList->Close();

	ID3D12GraphicsCommandList* graphCommandList = m_commandList;
	m_commandQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&graphCommandList);
	m_swapChain->Present(m_bVSync, 0);

	uint64 fenceValue = m_fenceValue +1;
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
	for (uint32 i = 0; i < s_frameCount; ++i)
	{
		SafeRelease(m_frameContext[i].CommandAllocator);
	}
	SafeRelease(m_commandQueue);
	SafeRelease(m_commandList);
	SafeRelease(m_srvDescHeap);
	SafeRelease(m_rtvHeap);
	SafeRelease(m_fence);
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

void Vacuum::DX12Renderer::LoadPipeline()
{
#if defined(_DEBUG)
	ComPtr<ID3D12Debug> debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif

	m_frameResources = new SFrameResource[s_frameCount];

	for (int32 i = 0; i < s_frameCount; ++i)
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

		for (uint32 i = 0; i < s_frameCount; ++i)
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

	for (int32 i = 0; i < s_frameCount; ++i)
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

void Vacuum::DX12Renderer::LoadAssets()
{
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
		ComPtr<ID3DBlob> vertexShader; 
		ComPtr<ID3DBlob> pixelShader;

#if defined(_DEBUG)
		uint32 compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		uint32 compileFlags = 0;
#endif
		/*for (const std::filesystem::path& shaderPath : m_shaderPaths)
		{*/
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
            PS_INPUT main(VS_INPUT input)\
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
            float4 main(PS_INPUT input) : SV_Target\
            {\
              float4 out_col = input.col * texture0.Sample(sampler0, input.uv); \
              return out_col; \
            }";

			/*ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, "vs_main", "vs_5_0", compileFlags, 0, &vertexShader, nullptr));
			ThrowIfFailed(D3DCompileFromFile(shaderPath.c_str(), nullptr, nullptr, "ps_main", "ps_5_0", compileFlags, 0, &pixelShader, nullptr));*/

			THROW_IF_FAILED(D3DCompile(vertexShaderStr, strlen(vertexShaderStr), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vertexShader, nullptr));
			THROW_IF_FAILED(D3DCompile(pixelShaderStr, strlen(pixelShaderStr), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &pixelShader, nullptr));

			static D3D12_INPUT_ELEMENT_DESC inputElementDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, pos), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,   0, (UINT)IM_OFFSETOF(ImDrawVert, uv),  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
				{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, (UINT)IM_OFFSETOF(ImDrawVert, col), D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
			};

			D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
			psoDesc.InputLayout = { inputElementDescs, 3 };
			psoDesc.pRootSignature = m_rootSignature;
			psoDesc.NodeMask = 1;
			psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
			psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
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
			//}

				THROW_IF_FAILED(m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState)));
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
		for (int32 adapterIndex = 0;
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

void Vacuum::DX12Renderer::CreateFontsTexture()
{
	// Build texture atlas
	ImGuiIO& io = ImGui::GetIO();
	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

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
		desc.Width = width;
		desc.Height = height;
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

		UINT uploadPitch = (width * 4 + D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u) & ~(D3D12_TEXTURE_DATA_PITCH_ALIGNMENT - 1u);
		UINT uploadSize = height * uploadPitch;
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
		IM_ASSERT(SUCCEEDED(hr));

		void* mapped = NULL;
		D3D12_RANGE range = { 0, uploadSize };
		hr = uploadBuffer->Map(0, &range, &mapped);
		IM_ASSERT(SUCCEEDED(hr));
		for (int y = 0; y < height; y++)
			memcpy((void*)((uintptr_t)mapped + y * uploadPitch), pixels + y * width * 4, width * 4);
		uploadBuffer->Unmap(0, &range);

		D3D12_TEXTURE_COPY_LOCATION srcLocation = {};
		srcLocation.pResource = uploadBuffer;
		srcLocation.Type = D3D12_TEXTURE_COPY_TYPE_PLACED_FOOTPRINT;
		srcLocation.PlacedFootprint.Footprint.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		srcLocation.PlacedFootprint.Footprint.Width = width;
		srcLocation.PlacedFootprint.Footprint.Height = height;
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
		IM_ASSERT(SUCCEEDED(hr));

		HANDLE event = CreateEvent(0, 0, 0, 0);
		IM_ASSERT(event != NULL);

		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.NodeMask = 1;

		ID3D12CommandQueue* cmdQueue = NULL;
		hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&cmdQueue));
		IM_ASSERT(SUCCEEDED(hr));

		ID3D12CommandAllocator* cmdAlloc = NULL;
		hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&cmdAlloc));
		IM_ASSERT(SUCCEEDED(hr));

		ID3D12GraphicsCommandList* cmdList = NULL;
		hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, cmdAlloc, NULL, IID_PPV_ARGS(&cmdList));
		IM_ASSERT(SUCCEEDED(hr));

		cmdList->CopyTextureRegion(&dstLocation, 0, 0, 0, &srcLocation, NULL);
		cmdList->ResourceBarrier(1, &barrier);

		hr = cmdList->Close();
		IM_ASSERT(SUCCEEDED(hr));

		cmdQueue->ExecuteCommandLists(1, (ID3D12CommandList* const*)&cmdList);
		hr = cmdQueue->Signal(fence, 1);
		IM_ASSERT(SUCCEEDED(hr));

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
	static_assert(sizeof(ImTextureID) >= sizeof(m_srvDescHeap->GetGPUDescriptorHandleForHeapStart().ptr), "Can't pack descriptor handle into TexID, 32-bit not supported yet.");
	io.Fonts->TexID = (ImTextureID)m_srvDescHeap->GetGPUDescriptorHandleForHeapStart().ptr;
}

Vacuum::SFrameContext* Vacuum::DX12Renderer::WaitForNextFrameResources()
{
	uint32 nextFrameIndex = m_frameIndex +1;
	m_frameIndex = nextFrameIndex;

	HANDLE waitableObjects[] = { m_swapChainWaitableObject, nullptr };
	DWORD numWaitableObjects = 1;

	SFrameContext* frameCtx = &m_frameContext[nextFrameIndex % s_frameCount];
	uint64 fenceValue = frameCtx->FenceValue;
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

void Vacuum::DX12Renderer::WaitForLastSubmittedFrame()
{
	SFrameContext* frameCtx = &m_frameContext[m_frameIndex % s_frameCount];

	uint64 fenceValue = frameCtx->FenceValue;
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

void Vacuum::DX12Renderer::SetupRenderState(ImDrawData* _drawData, SFrameResource* _frameResource)
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

	m_commandList->RSSetViewports(1, &viewPort);

	uint32 stride = sizeof(ImDrawVert);
	uint32 offset = 0;

	D3D12_VERTEX_BUFFER_VIEW vtxBufferView = {};
	vtxBufferView.BufferLocation = _frameResource->VertexBuffer->GetGPUVirtualAddress() + offset;
	vtxBufferView.SizeInBytes = _frameResource->VertexBufferSize * stride;
	vtxBufferView.StrideInBytes = stride;

	m_commandList->IASetVertexBuffers(0, 1, &vtxBufferView);

	D3D12_INDEX_BUFFER_VIEW idxBufferView = {};
	idxBufferView.BufferLocation = _frameResource->IndexBuffer->GetGPUVirtualAddress();
	idxBufferView.SizeInBytes = _frameResource->IndexBufferSize * sizeof(ImDrawIdx);
	idxBufferView.Format = sizeof(ImDrawIdx) == 2 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

	m_commandList->IASetIndexBuffer(&idxBufferView);
	m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_commandList->SetPipelineState(m_pipelineState);
	m_commandList->SetGraphicsRootSignature(m_rootSignature);
	m_commandList->SetGraphicsRoot32BitConstants(0, 16, &vertConstBuffer, 0);

	const float blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	m_commandList->OMSetBlendFactor(blendFactor);
}

void Vacuum::DX12Renderer::InvalidateObjects()
{
	if (!m_device)
	{
		return;
	}

	SafeRelease(m_rootSignature);
	SafeRelease(m_pipelineState);
	SafeRelease(m_fontTextureResource);

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->TexID = nullptr;

	for (uint32 i = 0; i < s_frameCount; ++i)
	{
		SFrameResource* frameResource = &m_frameResources[i];

		SafeRelease(frameResource->VertexBuffer);
		SafeRelease(frameResource->IndexBuffer);
	}
}

void Vacuum::DX12Renderer::CreateRenderTarget()
{
	for (uint32 i = 0; i < s_frameCount; ++i)
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

	for (uint32 i = 0; i < s_frameCount; ++i)
	{
		SafeRelease(m_renderTargets[i]);
	}
}

void Vacuum::DX12Renderer::ResizeSwapChain(HWND _hwnd, const uint32& _width, const uint32& _height)
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
