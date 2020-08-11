#pragma once


namespace Vacuum
{
	class CBaseObject
	{
	public:
		CBaseObject();
		virtual ~CBaseObject();
		virtual void Update(float _deltaseconds) {};
	};
}