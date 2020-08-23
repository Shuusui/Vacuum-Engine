#pragma once
#include "ECS\Header Files\BaseObject.h"
#include "Json.h"

namespace Vacuum
{
	class CBaseComponent : public CBaseObject
	{
	public: 
		CBaseComponent(const std::string& _name)
			:CBaseObject(_name)
		{

		}

		Json GetJsonObject() const
		{
			return jsonObject;
		}

		virtual void OnSave() = 0;
	protected:
		Json jsonObject;
	};

	class CTransformComponent : public CBaseComponent
	{
	public: 
		CTransformComponent()
			:CBaseComponent("Transform Component")
		{

		}

		CTransformComponent(const Json& _json)
			:CBaseComponent("Transform Component")
		{
			jsonObject = _json;
		}

		CTransformComponent(const CTransformComponent& _other)
			:CBaseComponent("Transform Component")
		{

		}

		virtual void OnSave() override
		{

		}
	};

	class CMeshComponent : public CBaseComponent
	{
	public: 
		CMeshComponent()
			:CBaseComponent("Mesh Component")
		{

		}

		virtual void OnSave() override
		{

		}
	};

	class CCameraComponent : public CBaseComponent
	{
	public: 
		CCameraComponent()
			:CBaseComponent("Camera Component")
		{

		}
		virtual void OnSave() override
		{

		}
	};
}