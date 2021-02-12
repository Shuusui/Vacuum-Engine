#pragma once
#include <vector>
#include "BaseObject.h"
#include "..\Entities\Header Files\BaseEntity.h"
#include <filesystem>

namespace Protostar
{
	namespace JsonKeys
	{
		constexpr const char* JSONSCENENAME = "name";
		constexpr const char* JSONSCENEGUID = "guid";
	}

	class CScene : public CBaseObject
	{
	public: 
		CScene(const std::string& _name)
			:CBaseObject(_name)
			,m_entities(std::vector<CBaseEntity*>())
			,m_cameraEntity(nullptr)
		{
		}

		CScene(const std::filesystem::path& _scenePath);

		virtual void OnSave() override;
		void RenderActiveCamera();
		void SetActiveCamera(CBaseEntity* _cameraEntity);
		void Delete();
	private:
		void LoadData();
		std::vector<CBaseEntity*> m_entities;
		CBaseEntity* m_cameraEntity;
	};
}