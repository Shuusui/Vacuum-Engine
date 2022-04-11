#ifndef BASEENTITY_H
#define BASEENTITY_H

#include "..\..\Header Files\BaseObject.h"
#include "ECS\Entt\Header Files\entt.hpp"

namespace Protostar
{
	class PBaseEntity : public PBaseObject
	{
	public:
		PBaseEntity(const std::string& _name);
		PBaseEntity(const std::filesystem::path& _path);
		virtual ~PBaseEntity();
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
#endif //BASEENTITY_H