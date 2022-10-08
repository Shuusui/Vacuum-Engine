#pragma once

#include "Guid.h"
#include <unordered_map>
#include <filesystem>
#include "Json.h"


namespace Protostar::Core
{
	//struct SamplerInfo
	//{
	//	std::string Name;
	//	std::vector<D3D12_STATIC_SAMPLER_DESC> SamplerDescs;
	//};

	//struct RootInfo
	//{
	//	SamplerInfo Info;
	//	ID3D12RootSignature* RootSignature;
	//};

	//class RootSignatureLibrary
	//{
	//public:
	//	static void Create(const std::filesystem::path& _projectPath);
	//	static void Destroy();
	//	static RootSignatureLibrary* GetHandle();

	//	bool CreateRootSignature(const SamplerInfo& _info);

	//	RootInfo GetRootInfo(const Guid& _guid) const
	//	{
	//		return m_rootInfos.at(_guid);
	//	}

	//	~RootSignatureLibrary();
	//private:
	//	RootSignatureLibrary(const std::filesystem::path& _projectPath);
	//	void Save();
	//	void Load();
	//	bool SerializeRootSignature(const std::vector<D3D12_STATIC_SAMPLER_DESC>& _descs, ID3DBlob*& _blob);

	//	static RootSignatureLibrary* s_rootSignatureLib;
	//	std::filesystem::path m_projectPath;
	//	std::filesystem::path m_rootSignatureLibIniPath;
	//	std::unordered_map<Guid, RootInfo> m_rootInfos;
	//};
}
