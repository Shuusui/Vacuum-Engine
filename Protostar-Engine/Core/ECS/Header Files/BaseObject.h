#pragma once
#include "Guid.h"
import <filesystem>;

namespace Protostar
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

		void SetObjectName(const std::string& _string)
		{
			if (m_oldName.empty())
			{
				m_oldName = m_objectName;
			}
			m_objectName = _string;
			if (m_oldPath.empty())
			{
				m_oldPath = m_objectPath;
			}
			m_objectPath = m_objectPath.replace_filename(_string);
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
		std::filesystem::path m_oldPath;
		SGuid m_guid;
		std::string m_objectName;
		std::string m_oldName;
	private: 
		std::string m_className;
	};
}