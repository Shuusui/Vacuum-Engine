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
			newScene->SetObjectPath(m_projectPaths.ScenesDir / (_name + ".pescene"));
			m_scenes.insert(newScene);
			CSavingSystem::GetHandle()->RegisterDirtyObject(newScene);
		}

		void CreateEntity(const std::string& _name)
		{
			CBaseEntity* newEntity = new CBaseEntity(_name);
			newEntity->SetObjectPath(m_projectPaths.EntitiesDir / (_name + ".peentity"));
			CEntityManager::GetHandle()->RegisterEntity(newEntity);
			CSavingSystem::GetHandle()->RegisterDirtyObject(newEntity);
		}

		void LoadScene(CScene* _scene)
		{
			if (m_currentScene)
			{
				PE_LOG_F("Unload current scene %s", m_currentScene->GetObjectName().c_str());
			}

			m_currentScene = _scene;
			PE_LOG_F("Load scene %s", _scene->GetObjectName().c_str());
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
		//std::unordered_map<std::string, SShaderInfo> m_shaders;
	};
}