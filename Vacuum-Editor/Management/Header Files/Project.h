#pragma once
#include <filesystem>
#include <unordered_map>
#include "Guid.h"
#include "Json.h"
#include "ECS\Header Files\Scene.h"
#include "ECS\Header Files\EntityManager.h"
#include "ECS\Entities\Header Files\BaseEntity.h"
#include "SavingSystem.h"
#include "Log.h"

namespace Vacuum
{
	struct SProjectPaths
	{
		std::filesystem::path ConfigDir;
		std::filesystem::path ProjectDir;
		std::filesystem::path ContentDir;
		std::filesystem::path ShaderDir;
		std::filesystem::path ModelsDir;
		std::filesystem::path ProjectFilePath;
		std::filesystem::path ScenesDir;
		std::filesystem::path EntitiesDir;
	};

	struct SShaderInfo
	{
		SShaderInfo(const std::filesystem::path& _filePath)
			:ShaderName(_filePath.filename().string())
		{

		}

		std::string ShaderName;

		Json ToJson() const
		{
			return Json{
				{"name", ShaderName}
			};
		}
	};

	struct SModelInfo
	{
		SModelInfo(const std::filesystem::path& _filePath)
			:ModelName(_filePath.filename().string())
		{

		}

		std::string ModelName;

		Json ToJson() const
		{
			return Json{
				{"name", ModelName}
			};
		}
	};

	class CProject
	{
	public:
		CProject(const std::filesystem::path& _projectPath);
		~CProject();

		SGuid GetGuid() const 
		{
			return m_guid;
		}

		std::string GetName() const
		{
			return m_name;
		}

		SProjectPaths GetProjectPaths() const 
		{
			return m_projectPaths;
		}

		void RegisterShader(const std::filesystem::path& _shaderPath)
		{
			m_shaders.insert(std::make_pair(_shaderPath.string(), SShaderInfo(_shaderPath)));
		}

		void RemoveShader(const std::filesystem::path& _shaderPath)
		{
			m_shaders.erase(_shaderPath.string());
		}

		std::unordered_map<std::string, SShaderInfo> GetShaders() const
		{
			return m_shaders;
		}

		void RegisterModel(const std::filesystem::path& _modelPath)
		{
			m_models.insert(std::make_pair(_modelPath.string(), SModelInfo(_modelPath)));
		}

		void RemoveModel(const std::filesystem::path& _modelPath)
		{
			m_models.erase(_modelPath.string());
		}

		std::unordered_map<std::string, SModelInfo> GetModels() const 
		{
			return m_models;
		}

		std::unordered_set<CScene*> GetScenes() const 
		{
			return m_scenes;
		}

		CScene* GetCurrentScene() const
		{
			return m_currentScene;
		}

		void CreateScene(const std::string& _name)
		{
			CScene* newScene = new CScene(_name);
			newScene->SetObjectPath(m_projectPaths.ScenesDir / (_name + ".vescene"));
			m_scenes.insert(newScene);
			CSavingSystem::GetHandle()->RegisterDirtyObject(newScene);
		}

		void CreateEntity(const std::string& _name)
		{
			CBaseEntity* newEntity = new CBaseEntity(_name);
			newEntity->SetObjectPath(m_projectPaths.EntitiesDir / (_name + ".veentity"));
			CEntityManager::GetHandle()->RegisterEntity(newEntity);
			CSavingSystem::GetHandle()->RegisterDirtyObject(newEntity);
		}

		void LoadScene(CScene* _scene)
		{
			if (m_currentScene)
			{
				VE_LOG_F("Unload current scene %s", m_currentScene->GetObjectName().c_str());
			}

			m_currentScene = _scene;
			VE_LOG_F("Load scene %s", _scene->GetObjectName().c_str());
		}

		void DeleteScene(CScene* _scene)
		{
			m_scenes.erase(_scene);
			if (m_currentScene == _scene)
			{
				m_currentScene = nullptr;
			}
			_scene->Delete();
			delete _scene;
			_scene = nullptr;
		}

	private:
		SGuid m_guid;
		CScene* m_currentScene;
		std::unordered_set<CScene*> m_scenes;
		std::string m_name;
		SProjectPaths m_projectPaths;
		std::unordered_map<std::string, SShaderInfo> m_shaders;
		std::unordered_map<std::string, SModelInfo> m_models;
	};
}