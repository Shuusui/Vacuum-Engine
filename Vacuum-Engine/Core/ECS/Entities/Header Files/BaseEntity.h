#pragma once
#include "..\..\Header Files\BaseObject.h"
#include "ECS\Entt\Header Files\entt.hpp"

namespace Vacuum
{
	class CBaseEntity : public CBaseObject
	{
	public:
		CBaseEntity(const std::string& _name);
		CBaseEntity(const std::filesystem::path& _path);
		virtual ~CBaseEntity();
		virtual void OnSave() override;
		void LoadData();
		entt::entity& GetEntity()
		{
			return m_entity;
		}
	private:
		entt::entity m_entity;
	};
}