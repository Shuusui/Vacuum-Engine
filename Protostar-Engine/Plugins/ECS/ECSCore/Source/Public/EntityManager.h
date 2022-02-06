#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

namespace Protostar
{
	class PBaseEntity;

	class CEntityManager
	{
	public: 
		static void CreateEntityManager();
		static CEntityManager* GetHandle()
		{
			return s_entityManager;
		}
		void RegisterEntity(PBaseEntity* _entity);
		std::vector<PBaseEntity*> GetEntities() const;
		~CEntityManager();
	private:
		CEntityManager();
		static CEntityManager* s_entityManager;
		std::vector<PBaseEntity*> m_entities;
	};
}
#endif //ENTITYMANAGER_H