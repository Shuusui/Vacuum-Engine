#pragma once
#include "Guid.h"
#include <filesystem>

namespace Vacuum
{
	class CBaseObject
	{
	public:
		CBaseObject(const std::string& _objectName);
		virtual ~CBaseObject();
		virtual void Update(float _deltaseconds) {};
		virtual void OnSave() = 0;
		void SetObjectPath(const std::filesystem::path& _objectPath)
		{
			m_objectPath = _objectPath;
		}

		SGuid GetGuid() const
		{
			return m_guid;
		}

		std::string GetObjectClassName() const
		{
			m_className;
		}

		std::string GetObjectName() const
		{
			return m_objectName;
		}

	protected:
		std::filesystem::path m_objectPath;
		SGuid m_guid;
		std::string m_objectName;
	private: 
		std::string m_className;
	};
}