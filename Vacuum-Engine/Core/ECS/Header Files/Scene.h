#pragma once
#include <vector>
#include "..\Entities\Header Files\BaseEntity.h"

namespace Vacuum
{
	class CScene
	{
	public: 
		CScene();
		~CScene();
	private:
		std::vector<CBaseEntity*> m_entities;
	};
}