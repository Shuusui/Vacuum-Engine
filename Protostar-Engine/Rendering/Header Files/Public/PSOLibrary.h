#pragma once
#include <unordered_map>
#include "Guid.h"

namespace Protostar
{
	class CPSOLibrary
	{
	public:
		static void Create();
		static CPSOLibrary* GetHandle()
		{
			return s_psoLibrary;
		}
		static void Destroy();

		~CPSOLibrary();
		SGuid CreatePSO();
	private:
		static CPSOLibrary* s_psoLibrary;
		CPSOLibrary();
		std::unordered_map<SGuid, void*> m_PSOs;
	};
}