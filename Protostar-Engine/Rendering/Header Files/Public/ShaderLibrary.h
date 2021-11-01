#pragma once
#include <unordered_map>
#include <filesystem>
#include <optional>
#include "Guid.h"
#include "Json.h"
#include "..\Private\DXHelper.h"
#include "Filesystem\Filesystem.h"

namespace Protostar
{
	struct SShaderInfo
	{
		SShaderInfo()
			:Name(std::string())
			,Shader(nullptr)
			,ShaderPath(std::filesystem::path())
		{
		}

		SShaderInfo(const SShaderInfo& _other)
			:Name(_other.Name)
			,Shader(_other.Shader)
			,ShaderPath(_other.ShaderPath)
		{
		}

		std::string Name;
		ID3DBlob* Shader;
		std::filesystem::path ShaderPath;
	};

	struct SShaderComplement
	{
		std::optional<SShaderInfo> VertexShaderInfo;
		std::optional<SShaderInfo> PixelShaderInfo;
	};

	class CShaderLibrary
	{
	public:
		~CShaderLibrary();

		static void Create(const std::filesystem::path& _projectPath);

		static CShaderLibrary* GetHandle()
		{
			return s_shaderLibrary;
		}

		static void Destroy();

		void Save();

		PJson ShaderInfoAsJson(const SShaderInfo& _shaderInfo) const;

		void LoadShaderJson();
		void LoadShaders(const std::filesystem::path& _shadersDirPath);

		bool LoadVertexShader(const std::filesystem::path& _shaderPath);
		bool LoadPixelShader(const std::filesystem::path& _shaderPath);
		bool LoadCombinedShader(const std::filesystem::path& _shaderPath);

		void UnloadVertexShader(const PGuid& _guid);
		void UnloadPixelShader(const PGuid& _guid);
		void UnloadShader(const PGuid& _guid);

		bool ContainsVertexShader(const PGuid& _guid) const;
		bool ContainsPixelShader(const PGuid& _guid) const;
		bool ContainsShader(const PGuid& _guid) const;

		SShaderInfo GetVertexShaderInfo(const PGuid& _guid) const;
		SShaderInfo GetPixelShaderInfo(const PGuid& _guid) const;
		/**
		 * returns a pair of shaderinfos where the first is vertex shader and the second is the pixel shader
		 */
		SShaderComplement GetShaderInfos(const PGuid& _guid) const;
		PTreeObject<SShaderComplement> GetShaderComplements() const;
	private:
		CShaderLibrary(const std::filesystem::path& _projectPath);

		static CShaderLibrary* s_shaderLibrary;

		std::filesystem::path m_projectConfigPath;
		std::filesystem::path m_shadersPath;
		std::filesystem::path m_shaderLibConfigPath;

		PTreeObject<SShaderComplement> m_fileTree;

		std::unordered_map<PGuid, SShaderInfo> m_vertexShaders;
		std::unordered_map<PGuid, SShaderInfo> m_pixelShaders;

		std::unordered_map<std::string, PGuid> m_vertexShaderNames;
		std::unordered_map<std::string, PGuid> m_pixelShaderNames;
	};
}