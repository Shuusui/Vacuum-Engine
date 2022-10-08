//#pragma once
//
//#include <unordered_map>
//#include <filesystem>
//#include <optional>
//#include "Guid.h"
//#include "Json.h"
//#include "Filesystem.h"
//
//namespace Protostar::Core
//{
//	struct ShaderInfo
//	{
//		ShaderInfo()
//			:Name(std::string())
//			,Shader(nullptr)
//			,ShaderPath(std::filesystem::path())
//		{
//		}
//
//		ShaderInfo(const ShaderInfo& _other)
//			:Name(_other.Name)
//			,Shader(_other.Shader)
//			,ShaderPath(_other.ShaderPath)
//		{
//		}
//
//		std::string Name;
//		void* Shader;
//		std::filesystem::path ShaderPath;
//	};
//
//	struct ShaderComplement
//	{
//		std::optional<ShaderInfo> VertexShaderInfo;
//		std::optional<ShaderInfo> PixelShaderInfo;
//	};
//
//	class ShaderLibrary
//	{
//	public:
//		~ShaderLibrary();
//
//		static void Create(const std::filesystem::path& _projectPath);
//
//		static ShaderLibrary* GetHandle()
//		{
//			return s_shaderLibrary;
//		}
//
//		static void Destroy();
//
//		void Save();
//
//		void LoadShaderJson();
//		void LoadShaders(const std::filesystem::path& _shadersDirPath);
//
//		bool LoadVertexShader(const std::filesystem::path& _shaderPath);
//		bool LoadPixelShader(const std::filesystem::path& _shaderPath);
//		bool LoadCombinedShader(const std::filesystem::path& _shaderPath);
//
//		void UnloadVertexShader(const Guid& _guid);
//		void UnloadPixelShader(const Guid& _guid);
//		void UnloadShader(const Guid& _guid);
//
//		bool ContainsVertexShader(const Guid& _guid) const;
//		bool ContainsPixelShader(const Guid& _guid) const;
//		bool ContainsShader(const Guid& _guid) const;
//
//		ShaderInfo GetVertexShaderInfo(const Guid& _guid) const;
//		ShaderInfo GetPixelShaderInfo(const Guid& _guid) const;
//		/**
//		 * returns a pair of shaderinfos where the first is vertex shader and the second is the pixel shader
//		 */
//		ShaderComplement GetShaderInfos(const Guid& _guid) const;
//		TreeObject<ShaderComplement> GetShaderComplements() const;
//	private:
//		ShaderLibrary(const std::filesystem::path& _projectPath);
//
//		static ShaderLibrary* s_shaderLibrary;
//
//		std::filesystem::path m_projectConfigPath;
//		std::filesystem::path m_shadersPath;
//		std::filesystem::path m_shaderLibConfigPath;
//
//		TreeObject<ShaderComplement> m_fileTree;
//
//		std::unordered_map<Guid, ShaderInfo> m_vertexShaders;
//		std::unordered_map<Guid, ShaderInfo> m_pixelShaders;
//
//		std::unordered_map<std::string, Guid> m_vertexShaderNames;
//		std::unordered_map<std::string, Guid> m_pixelShaderNames;
//	};
//}
