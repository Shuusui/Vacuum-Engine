//#include "PSOLibrary.h"
//#include "d3dx12.h"
//#include "RendererManager.h"
//#include "DXHelper.h"
//#include "Json.h"
//#include "ShaderLibrary.h"
//#include "RootSignatureLibrary.h"
//#include <fstream>
//
//namespace Protostar::Core
//{
//
//	PSOLibrary* PSOLibrary::s_psoLibrary = nullptr;
//
//
//	void PSOLibrary::Create(const std::filesystem::path& _projectPath)
//	{
//		if (s_psoLibrary)
//		{
//			return;
//		}
//		s_psoLibrary = new PSOLibrary(_projectPath);
//	}
//
//	void PSOLibrary::Destroy()
//	{
//		if (s_psoLibrary)
//		{
//			delete s_psoLibrary;
//			s_psoLibrary = nullptr;
//		}
//	}
//
//	PSOLibrary::~PSOLibrary()
//	{
//	}
//
//	Guid PSOLibrary::CreatePSO(const PPSOInfo& _psoInfo)
//	{
//		D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
//		psoDesc.InputLayout = { _psoInfo.InputElementDescs.data(), (UINT)_psoInfo.InputElementDescs.size() };
//		psoDesc.pRootSignature = _psoInfo.RootSignature.second;
//		psoDesc.VS = CD3DX12_SHADER_BYTECODE(_psoInfo.VertexShader.second);
//		psoDesc.PS = CD3DX12_SHADER_BYTECODE(_psoInfo.PixelShader.second);
//		psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(_psoInfo.RasterizerDesc);
//		psoDesc.BlendState = CD3DX12_BLEND_DESC(_psoInfo.BlendDesc);
//		psoDesc.DepthStencilState.DepthEnable = FALSE;
//		psoDesc.DepthStencilState.StencilEnable = FALSE;
//		psoDesc.SampleMask = UINT_MAX;
//		psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//		psoDesc.NumRenderTargets = 1;
//		psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
//		psoDesc.SampleDesc.Count = 1;
//		psoDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
//
//		if (!RendererManager::CreatePSO(&psoDesc, (void**)_psoInfo.PipelineState))
//		{
//			return Guid();
//		}
//		Guid guid = Guid::NewGuid();
//		m_PSOs.insert(std::make_pair(guid, _psoInfo));
//		return guid;
//	}
//
//	void PSOLibrary::Load()
//	{
//	}
//
//	void PSOLibrary::Save()
//	{
//	}
//
//	void PSOLibrary::CreateDefaultPSO()
//	{
//
//	}
//
//	PSOLibrary::PSOLibrary(const std::filesystem::path& _projectPath)
//		:m_projectPath(_projectPath)
//		, m_projectConfigPath(_projectPath / "Configs")
//		, m_psoLibraryIniPath(m_projectConfigPath / "psolibrary.ini")
//	{
//		if (std::filesystem::exists(m_psoLibraryIniPath))
//		{
//			Load();
//		}
//		else
//		{
//			CreateDefaultPSO();
//		}
//	}
//}