#pragma once
#include "d3d12.h"
#include "Guid.h"
#include <unordered_map>
#include <filesystem>
#include "Json.h"

#define JSONROOTNAME "name"
#define JSONSAMPLERDESCS "sampler_descs"
#define JSONSAMPLERDESCU "adress_u"
#define JSONSAMPLERDESCV "adress_v"
#define JSONSAMPLERDESCW "adress_w"
#define JSONSAMPLERDESCMINLOD "min_lod"
#define JSONSAMPLERDESCMAXLOD "max_lod"
#define JSONSAMPLERDESCMIPLODBIAS "mip_lod_bias"
#define JSONSAMPLERDESCMAXANISOTROPY "max_anisotropy"
#define JSONSAMPLERDESCSHADERVISIBILITY "shader_visibility"
#define JSONSAMPLERDESCFILTER "filter"
#define JSONSAMPLERDESCSHADERREGISTER "shader_register"
#define JSONSAMPLERDESCREGISTERSPACE "register_space"
#define JSONSAMPLERDESCCOMPFUNC "comp_func"
#define JSONSAMPLERDESCBORDERCOLOR "border_color"

namespace Protostar
{
	struct PSamplerInfo
	{
		std::string Name;
		std::vector<D3D12_STATIC_SAMPLER_DESC> SamplerDescs;

		PJson ToJson() const
		{
			PJson json = {};

			json[JSONROOTNAME] = Name.c_str();

			std::vector<PJson> samplerDescJsons = {};

			for (const D3D12_STATIC_SAMPLER_DESC& samplerDesc : SamplerDescs)
			{
				samplerDescJsons.push_back(SamplerDescToJson(samplerDesc));
			}

			json[JSONSAMPLERDESCS] = samplerDescJsons;
			return json;
		}

		void FromJson(const PJson& _json)
		{
			Name = _json[JSONROOTNAME].get<std::string>();
			for (const PJson& json : _json[JSONSAMPLERDESCS].get<std::vector<PJson>>())
			{
				SamplerDescs.push_back(JsonToSamplerDesc(json));
			}
		}

	private: 
		D3D12_STATIC_SAMPLER_DESC JsonToSamplerDesc(const PJson& _json) const
		{
			return D3D12_STATIC_SAMPLER_DESC
			{
				(D3D12_FILTER)_json[JSONSAMPLERDESCFILTER].get<u32>(),
				(D3D12_TEXTURE_ADDRESS_MODE)_json[JSONSAMPLERDESCU].get<u32>(), 
				(D3D12_TEXTURE_ADDRESS_MODE)_json[JSONSAMPLERDESCV].get<u32>(),
				(D3D12_TEXTURE_ADDRESS_MODE)_json[JSONSAMPLERDESCW].get<u32>(),
				_json[JSONSAMPLERDESCMIPLODBIAS].get<float>(), 
				_json[JSONSAMPLERDESCMAXANISOTROPY].get<u32>(),
				(D3D12_COMPARISON_FUNC)_json[JSONSAMPLERDESCCOMPFUNC].get<u32>(),
				(D3D12_STATIC_BORDER_COLOR)_json[JSONSAMPLERDESCBORDERCOLOR].get<u32>(),
				_json[JSONSAMPLERDESCMINLOD].get<float>(),
				_json[JSONSAMPLERDESCMAXLOD].get<float>(),
				_json[JSONSAMPLERDESCSHADERREGISTER].get<u32>(),
				_json[JSONSAMPLERDESCREGISTERSPACE].get<u32>(),
				(D3D12_SHADER_VISIBILITY)_json[JSONSAMPLERDESCSHADERVISIBILITY].get<u32>()
			};
		}
		PJson SamplerDescToJson(const D3D12_STATIC_SAMPLER_DESC& _desc) const
		{
			return PJson
			{
				{JSONSAMPLERDESCU, _desc.AddressU},
				{JSONSAMPLERDESCV, _desc.AddressV},
				{JSONSAMPLERDESCW, _desc.AddressW},
				{JSONSAMPLERDESCMINLOD, _desc.MinLOD},
				{JSONSAMPLERDESCMAXLOD, _desc.MaxLOD},
				{JSONSAMPLERDESCMIPLODBIAS, _desc.MipLODBias},
				{JSONSAMPLERDESCMAXANISOTROPY, _desc.MaxAnisotropy},
				{JSONSAMPLERDESCSHADERVISIBILITY, _desc.ShaderVisibility},
				{JSONSAMPLERDESCFILTER, _desc.Filter},
				{JSONSAMPLERDESCSHADERREGISTER, _desc.ShaderRegister},
				{JSONSAMPLERDESCREGISTERSPACE, _desc.RegisterSpace},
				{JSONSAMPLERDESCCOMPFUNC, _desc.ComparisonFunc}, 
				{JSONSAMPLERDESCBORDERCOLOR, _desc.BorderColor}
			};
		}
	};

	struct PRootInfo
	{
		PSamplerInfo Info;
		ID3D12RootSignature* RootSignature;
	};

	class PRootSignatureLibrary
	{
	public:
		static void Create(const std::filesystem::path& _projectPath);
		static void Destroy();
		static PRootSignatureLibrary* GetHandle();

		bool CreateRootSignature(const PSamplerInfo& _info);
		std::unordered_map<PGuid, PRootInfo> GetRootInfos() const
		{
			return m_rootInfos;
		}
		PRootInfo GetRootInfo(const PGuid& _guid) const
		{
			return m_rootInfos.at(_guid);
		}

		~PRootSignatureLibrary();
	private:
		PRootSignatureLibrary(const std::filesystem::path& _projectPath);
		void Save();
		void Load();
		bool SerializeRootSignature(const std::vector<D3D12_STATIC_SAMPLER_DESC>& _descs, ID3DBlob*& _blob);

		static PRootSignatureLibrary* s_rootSignatureLib;
		std::filesystem::path m_projectPath;
		std::filesystem::path m_rootSignatureLibIniPath;
		std::unordered_map<PGuid, PRootInfo> m_rootInfos;
	};
}


#undef JSONROOTNAME
#undef JSONSAMPLERDESCU 
#undef JSONSAMPLERDESCV 
#undef JSONSAMPLERDESCW 
#undef JSONSAMPLERDESCMINLOD 
#undef JSONSAMPLERDESCMAXLOD 
#undef JSONSAMPLERDESCMIPLODBIAS 
#undef JSONSAMPLERDESCMAXANISOTROPY 
#undef JSONSAMPLERDESCSHADERVISIBILITY 
#undef JSONSAMPLERDESCFILTER 
#undef JSONSAMPLERDESCSHADERREGISTER 
#undef JSONSAMPLERDESCREGISTERSPACE 
#undef JSONSAMPLERDESCCOMPFUNC 
#undef JSONSAMPLERDESCBORDERCOLOR 