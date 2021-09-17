#pragma once
#include "BaseObject.h"
#include "..\Entities\Header Files\BaseEntity.h"
import <filesystem>;
import <vector>;

namespace Protostar
{
	class CScene : public CBaseObject
	{
	public: 
		CScene(const std::string& _name)
			:CBaseObject(_name)
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