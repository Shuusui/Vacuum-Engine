#pragma once
#include <string>
#include "GlobalDefs.h"
#include "SharedStructs.h"

namespace Vacuum
{
	class CApp
	{
	public:
		static void InitApp();
		static void Destroy();
		static CApp* GetAppHandle();

		SWindowDimParams GetInitWindowDimParams() const;
	private:
		CApp();
		
		static CApp* s_app;
		SWindowDimParams m_mainWindowDim;
	};
}