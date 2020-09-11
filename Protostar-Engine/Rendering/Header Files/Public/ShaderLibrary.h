#pragma once
#include <unordered_map>
#include <filesystem>
#include <optional>
#include "Guid.h"
#include "Json.h"
#include "..\Private\DXHelper.h"

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

		Json ShaderInfoAsJson(const SShaderInfo& _shaderInfo) const;

		void LoadShaderJson();
		void LoadShaders(const std::filesystem::path& _shadersDirPath);

		void LoadVertexShader(const std::filesystem::path& _shaderPath);
		void LoadPixelShader(const std::filesystem::path& _shaderPath);
		void LoadCombinedShader(const std::filesystem::path& _shaderPath);

		void UnloadVertexShader(const SGuid& _guid);
		void UnloadPixelShader(const SGuid& _guid);
		void UnloadShader(const SGuid& _guid);

		bool ContainsVertexShader(const SGuid& _guid) const;
		bool ContainsPixelShader(const SGuid& _guid) const;
		bool ContainsShader(const SGuid& _guid) const;

		SShaderInfo GetVertexShaderInfo(const SGuid& _guid) const;
		SShaderInfo GetPixelShaderInfo(const SGuid& _guid) const;
		/**
		 * returns a pair of shaderinfos where the first is vertex shader and the second is the pixel shader
		 */
		SShaderComplement GetShaderInfos(const SGuid& _guid) const;
		std::unordered_map<SGuid, SShaderComplement> GetShaderComplements() const;
	private:
		CShaderLibrary(const std::filesystem::path& _projectPath);

		static CShaderLibrary* s_shaderLibrary;

		std::filesystem::path m_projectConfigPath;
		std::filesystem::path m_shaderLibConfigPath;

		std::unordered_map<SGuid, SShaderInfo> m_vertexShaders;
		std::unordered_map<SGuid, SShaderInfo> m_pixelShaders;

		std::unordered_map<std::string, SGuid> m_vertexShaderNames;
		std::unordered_map<std::string, SGuid> m_pixelShaderNames;
	};
}