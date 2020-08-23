#pragma once
#include <unordered_set>
#include <vector>
#include "ECS\Header Files\BaseObject.h"

namespace Vacuum
{
	class CSavingSystem
	{
	public:
		static void OnCreate();
		static void OnDestroy();
		static CSavingSystem* GetHandle()
		{
			return s_savingSystem;
		}

		void RegisterDirtyObject(CBaseObject* _object)
		{
			m_dirtyObjects.insert(_object);
		}

		void SaveAllDirtyObjects()
		{
			for (CBaseObject* dirtyObject : m_dirtyObjects)
			{
				if (!dirtyObject)
				{
					continue;
				}
				dirtyObject->OnSave();
			}
			m_dirtyObjects.clear();
		}

		bool HasDirtyObjects() const
		{
			return m_dirtyObjects.size() > 0;
		}

		void SaveDirtyObjects(const std::vector<CBaseObject*>& _objectsToSave)
		{
			for (CBaseObject* objectToSave : _objectsToSave)
			{
				if (!objectToSave)
				{
					continue;
				}
				objectToSave->OnSave();
				m_dirtyObjects.erase(objectToSave);
			}
		}

		std::unordered_set<CBaseObject*> GetDirtyObjects() const
		{
			return m_dirtyObjects;
		}
	private:
		static CSavingSystem* s_savingSystem;
		std::unordered_set<CBaseObject*> m_dirtyObjects;
	};
}