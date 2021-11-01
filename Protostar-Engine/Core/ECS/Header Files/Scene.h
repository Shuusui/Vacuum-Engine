#pragma once
#include <vector>
#include "BaseObject.h"
#include "..\Entities\Header Files\BaseEntity.h"
#include <filesystem>

namespace Protostar
{
	class PScene : public PBaseObject
	{
	public: 
		PScene(const std::string& _name)
			:PBaseObject(_name)
		{
		}

		PScene(const std::filesystem::path& _scenePath);

		virtual void OnSave() override;
		void RenderActiveCamera();
		void SetActiveCamera(PBaseEntity* _cameraEntity);
		void Delete();
	private:
		void LoadData();
		std::vector<PBaseEntity*> m_entities;
		PBaseEntity* m_cameraEntity;
	};
}