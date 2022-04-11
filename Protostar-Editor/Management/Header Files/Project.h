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

namespace Protostar
{
	struct PProjectPaths
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

	class PProject
	{
	public:
		PProject(const std::filesystem::path& _projectPath);
		~PProject();

		PGuid GetGuid() const 
		{
			return m_guid;
		}

		std::string GetName() const
		{
			return m_name;
		}

		PProjectPaths GetProjectPaths() const 
		{
			return m_projectPaths;
		}

		std::unordered_set<PScene*> GetScenes() const 
		{
			return m_scenes;
		}

		PScene* GetCurrentScene() const
		{
			return m_currentScene;
		}

		void CreateScene(const std::string& _name)
		{
			PScene* newScene = new PScene(_name);
			newScene->SetObjectPath(m_projectPaths.ScenesDir / (_name + ".pescene"));
			m_scenes.insert(newScene);
			PSavingSystem::GetHandle()->RegisterDirtyObject(newScene);
		}

		void CreateEntity(const std::string& _name)
		{
			PBaseEntity* newEntity = new PBaseEntity(_name);
			newEntity->SetObjectPath(m_projectPaths.EntitiesDir / (_name + ".peentity"));
			CEntityManager::GetHandle()->RegisterEntity(newEntity);
			PSavingSystem::GetHandle()->RegisterDirtyObject(newEntity);
		}

		void LoadScene(PScene* _scene)
		{
			if (m_currentScene)
			{
				PE_LOG_F("Unload current scene %s", m_currentScene->GetObjectName().c_str());
			}

			m_currentScene = _scene;
			PE_LOG_F("Load scene %s", _scene->GetObjectName().c_str());
		}

		void DeleteScene(PScene* _scene)
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
		PGuid m_guid;
		PScene* m_currentScene;
		std::unordered_set<PScene*> m_scenes;
		std::string m_name;
		PProjectPaths m_projectPaths;
		//std::unordered_map<std::string, SShaderInfo> m_shaders;
	};
}