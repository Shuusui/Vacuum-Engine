#include "..\Header Files\Public\RootSignatureLibrary.h"
#include "Log.h"
#include "DXHelper.h"
#include "RendererManager.h"
#include <fstream>

Protostar::PRootSignatureLibrary* Protostar::PRootSignatureLibrary::s_rootSignatureLib = nullptr;

#define JSONROOTINFO "info"

void Protostar::PRootSignatureLibrary::Create(const std::filesystem::path& _projectPath)
{
	if (s_rootSignatureLib)
	{
		return;
	}

	s_rootSignatureLib = new PRootSignatureLibrary(_projectPath);
}

void Protostar::PRootSignatureLibrary::Destroy()
{
	if (s_rootSignatureLib)
	{
		delete s_rootSignatureLib; 
		s_rootSignatureLib = nullptr;
	}
}

Protostar::PRootSignatureLibrary* Protostar::PRootSignatureLibrary::GetHandle()
{
	return s_rootSignatureLib;
}

bool Protostar::PRootSignatureLibrary::CreateRootSignature(const PSamplerInfo& _info)
{
	ID3D12RootSignature* rootSignature = nullptr;

	ID3DBlob* blob = nullptr;

	if (!SerializeRootSignature(_info.SamplerDescs, blob))
	{
		return false;
	}

	if (!PRendererManager::CreateRootSignature(blob, (void**)&rootSignature))
	{
		return false;
	}

	m_rootInfos.insert(std::make_pair(PGuid::NewGuid(), PRootInfo{_info, rootSignature}));

	return true;
}

Protostar::PRootSignatureLibrary::~PRootSignatureLibrary()
{
	if (!m_rootInfos.empty())
	{
		Save();
	}
}

Protostar::PRootSignatureLibrary::PRootSignatureLibrary(const std::filesystem::path& _projectPath)
	: m_projectPath(_projectPath)
	, m_rootSignatureLibIniPath(_projectPath / "Configs" / "rootsignaturelib.ini")
	, m_rootInfos({})
{
	if (std::filesystem::exists(m_rootSignatureLibIniPath))
	{
		Load();
	}
}

void Protostar::PRootSignatureLibrary::Save()
{
}

void Protostar::PRootSignatureLibrary::Load()
{
}

bool Protostar::PRootSignatureLibrary::SerializeRootSignature(const std::vector<D3D12_STATIC_SAMPLER_DESC>& _descs, ID3DBlob*& _blob)
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

	D3D12_ROOT_SIGNATURE_DESC desc = {};
	desc.NumParameters = _countof(param);
	desc.pParameters = param;
	desc.NumStaticSamplers = 1;
	desc.pStaticSamplers = _descs.data();
	desc.Flags =
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_HULL_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS |
		D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS;

	ID3DBlob* errorBlob = nullptr;
	if (D3D12SerializeRootSignature(&desc, D3D_ROOT_SIGNATURE_VERSION_1, &_blob, &errorBlob) != S_OK)
	{
		PE_LOG(std::string((char*)errorBlob->GetBufferPointer()).c_str());
		SafeRelease(_blob);
		SafeRelease(errorBlob);
		return false;
	}
	SafeRelease(errorBlob);
	return true;
}

#undef JSONROOTGUID
#undef JSONROOTINFO