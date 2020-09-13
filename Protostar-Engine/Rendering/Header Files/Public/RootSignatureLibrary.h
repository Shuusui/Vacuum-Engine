#pragma once
#include "d3dx12.h"
#include "Guid.h"
#include <unordered_map>
#include <filesystem>
#include "Json.h"


#define JSONROOTNAME "name"

namespace Protostar
{
	struct SSamplerInfo
	{
		std::string Name;
		std::vector<D3D12_STATIC_SAMPLER_DESC> SamplerDescs;

		Json ToJson() const
		{

		}

		void FromJson(const Json& _json)
		{

		}
	};

	struct SRootInfo
	{
		SSamplerInfo Info;
		ID3D12RootSignature* RootSignature;
	};

	class CRootSignatureLibrary
	{
	public:
		static void Create(const std::filesystem::path& _projectPath);
		static void Destroy();
		static CRootSignatureLibrary* GetHandle();

		bool CreateRootSignature(const SSamplerInfo& _info);
		std::unordered_map<SGuid, SRootInfo> GetRootSignatures() const
		{
			return m_rootInfos;
		}
		SRootInfo GetRootInfo(const SGuid& _guid) const
		{
			return m_rootInfos.at(_guid);
		}

		~CRootSignatureLibrary();
	private:
		CRootSignatureLibrary(const std::filesystem::path& _projectPath);
		void Save();
		void Load();
		bool SerializeRootSignature(const std::vector<D3D12_STATIC_SAMPLER_DESC>& _descs, ID3DBlob* _blob);

		static CRootSignatureLibrary* s_rootSignatureLib;
		std::filesystem::path m_projectPath;
		std::filesystem::path m_rootSignatureLibIniPath;
		std::unordered_map<SGuid, SRootInfo> m_rootInfos;
	};
}


#undef JSONROOTNAME