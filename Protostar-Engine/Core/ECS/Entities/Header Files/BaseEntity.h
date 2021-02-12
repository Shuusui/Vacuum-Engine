#pragma once
#include "..\..\Header Files\BaseObject.h"
#include "ECS\Entt\Header Files\entt.hpp"

namespace Protostar
{
	namespace JsonKeys
	{
		constexpr const char* JSONENTITYNAME =		"name";
		constexpr const char* JSONENTITYGUID =		"guid";
		constexpr const char* JSONTRANSFORM =		"transform";
		constexpr const char* JSONMESH =			"mesh";
	}

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