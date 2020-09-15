#pragma once
#include <vector>

namespace Protostar
{
	class CBaseEntity;

	class CEntityManager
	{
	public: 
		static void CreateEntityManager();
		static CEntityManager* GetHandle()
		{
			return s_entityManager;
		}
		void RegisterEntity(CBaseEntity* _entity);
		std::vector<CBaseEntity*> GetEntities() const;
		~CEntityManager();
	private:
		CEntityManager();
		static CEntityManager* s_entityManager;
		std::vector<CBaseEntity*> m_entities;
	};
}