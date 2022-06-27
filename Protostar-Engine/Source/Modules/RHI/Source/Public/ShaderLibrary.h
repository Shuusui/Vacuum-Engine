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
	struct PShaderInfo
	{
		PShaderInfo()
			:Name(std::string())
			,Shader(nullptr)
			,ShaderPath(std::filesystem::path())
		{
		}

		PShaderInfo(const PShaderInfo& _other)
			:Name(_other.Name)
			,Shader(_other.Shader)
			,ShaderPath(_other.ShaderPath)
		{
		}

		std::string Name;
		ID3DBlob* Shader;
		std::filesystem::path ShaderPath;
	};

	struct PShaderComplement
	{
		std::optional<PShaderInfo> VertexShaderInfo;
		std::optional<PShaderInfo> PixelShaderInfo;
	};

	class PShaderLibrary
	{
	public:
		~PShaderLibrary();

		static void Create(const std::filesystem::path& _projectPath);

		static PShaderLibrary* GetHandle()
		{
			return s_shaderLibrary;
		}

		static void Destroy();

		void Save();

		PJson ShaderInfoAsJson(const PShaderInfo& _shaderInfo) const;

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

		PShaderInfo GetVertexShaderInfo(const PGuid& _guid) const;
		PShaderInfo GetPixelShaderInfo(const PGuid& _guid) const;
		/**
		 * returns a pair of shaderinfos where the first is vertex shader and the second is the pixel shader
		 */
		PShaderComplement GetShaderInfos(const PGuid& _guid) const;
		PTreeObject<PShaderComplement> GetShaderComplements() const;
	private:
		PShaderLibrary(const std::filesystem::path& _projectPath);

		static PShaderLibrary* s_shaderLibrary;

		std::filesystem::path m_projectConfigPath;
		std::filesystem::path m_shadersPath;
		std::filesystem::path m_shaderLibConfigPath;

		PTreeObject<PShaderComplement> m_fileTree;

		std::unordered_map<PGuid, PShaderInfo> m_vertexShaders;
		std::unordered_map<PGuid, PShaderInfo> m_pixelShaders;

		std::unordered_map<std::string, PGuid> m_vertexShaderNames;
		std::unordered_map<std::string, PGuid> m_pixelShaderNames;
	};
}
