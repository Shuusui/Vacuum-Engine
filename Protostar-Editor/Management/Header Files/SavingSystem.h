#pragma once
#include <unordered_set>
#include <vector>
#include "ECS\Header Files\BaseObject.h"

namespace Protostar
{
	class PSavingSystem
	{
	public:
		static void OnCreate();
		static void OnDestroy();
		static PSavingSystem* GetHandle()
		{
			return s_savingSystem;
		}

		void RegisterDirtyObject(PBaseObject* _object)
		{
			m_dirtyObjects.insert(_object);
		}

		void SaveAllDirtyObjects()
		{
			for (PBaseObject* dirtyObject : m_dirtyObjects)
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

		void SaveDirtyObjects(const std::vector<PBaseObject*>& _objectsToSave)
		{
			for (PBaseObject* objectToSave : _objectsToSave)
			{
				if (!objectToSave)
				{
					continue;
				}
				objectToSave->OnSave();
				m_dirtyObjects.erase(objectToSave);
			}
		}

		std::unordered_set<PBaseObject*> GetDirtyObjects() const
		{
			return m_dirtyObjects;
		}
	private:
		static PSavingSystem* s_savingSystem;
		std::unordered_set<PBaseObject*> m_dirtyObjects;
	};
}