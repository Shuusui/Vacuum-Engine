#pragma once

#include "d3d12.h"
#include "Guid.h"
#include <unordered_map>
#include <filesystem>
#include "Json.h"


namespace Protostar
{
	struct PSamplerInfo
	{
		std::string Name;
		std::vector<D3D12_STATIC_SAMPLER_DESC> SamplerDescs;
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
