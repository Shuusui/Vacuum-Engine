#pragma once
#include <vector>
#include "BaseObject.h"
#include "..\Entities\Header Files\BaseEntity.h"
#include <filesystem>

namespace Vacuum
{
	class CScene : public CBaseObject
	{
	public: 
		CScene(const std::string& _name)
			:CBaseObject(_name)
		{
		}

		CScene(const std::filesystem::path& _scenePath);

		virtual void OnSave();
		void Delete();
	private:
		void LoadData();
		std::vector<CBaseEntity*> m_entities;
	};
}