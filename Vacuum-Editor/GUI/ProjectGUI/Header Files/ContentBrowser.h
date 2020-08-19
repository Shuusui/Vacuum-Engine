#pragma once
#include <filesystem>
#include <set>
#include "Guid.h"

namespace Vacuum
{
	struct SShaderInfo
	{
		SShaderInfo(const std::filesystem::path& _shaderPath)
			:ShaderPath(_shaderPath)
			,bIsSelected(false)
		{

		}

		std::filesystem::path ShaderPath;
		bool bIsSelected;

		bool operator==(const std::filesystem::path& _other) const
		{
			return ShaderPath == _other;
		}

		bool operator!=(const std::filesystem::path& _other) const 
		{
			return ShaderPath != _other;
		}
	};

	class CContentBrowser
	{
	public:
		CContentBrowser(const std::filesystem::path& _contentDir);
		void OnRender();
	private:
		std::filesystem::path m_contentDir;
		std::filesystem::path m_shaderDir;
		std::filesystem::path m_modelsDir;
		std::filesystem::path m_scenesDir;

		std::vector<SShaderInfo> m_shaders;
	};
}