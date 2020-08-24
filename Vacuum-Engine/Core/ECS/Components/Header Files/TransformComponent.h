#pragma once
#include "BaseComponent.h"
#include <DirectXMath.h>

#define JSONPOSX "x"
#define JSONPOSY "y"
#define JSONPOSZ "z"

#define JSONROTPITCH "pitch"
#define JSONROTYAW "yaw"
#define JSONROTROLL "roll"

#define JSONSCALEX "x"
#define JSONSCALEY "y"
#define JSONSCALEZ "z"

#define JSONPOS "position"
#define JSONROT "rotation"
#define JSONSCALE "scale"

namespace Vacuum
{
	class CTransformComponent : public CBaseComponent
	{
	public:
		CTransformComponent()
			:CBaseComponent("Transform Component")
			,m_position(DirectX::XMFLOAT3())
			,m_rotation(DirectX::XMFLOAT3())
			,m_scale(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f))
		{

		}

		CTransformComponent(const Json& _json)
			:CBaseComponent("Transform Component")
			,m_position(DirectX::XMFLOAT3())
			,m_rotation(DirectX::XMFLOAT3())
			,m_scale(DirectX::XMFLOAT3())
		{
			m_jsonObject = _json;
			if (m_jsonObject.contains(JSONPOS))
			{
				Json posJson = m_jsonObject[JSONPOS].get<Json>();
				m_position.x = posJson[JSONPOSX].get<float>();
				m_position.y = posJson[JSONPOSY].get<float>();
				m_position.z = posJson[JSONPOSZ].get<float>();
			}

			if (m_jsonObject.contains(JSONROT))
			{
				Json rotJson = m_jsonObject[JSONROT].get<Json>();
				m_rotation.x = rotJson[JSONROTPITCH].get<float>();
				m_rotation.y = rotJson[JSONROTYAW].get<float>();
				m_rotation.z = rotJson[JSONROTROLL].get<float>();
			}

			if (m_jsonObject.contains(JSONSCALE))
			{
				Json scaleJson = m_jsonObject[JSONSCALE].get<Json>();
				m_scale.x = scaleJson[JSONSCALEX].get<float>();
				m_scale.y = scaleJson[JSONSCALEY].get<float>();
				m_scale.z = scaleJson[JSONSCALEZ].get<float>();
			}
		}

		CTransformComponent(const CTransformComponent& _other)
			:CBaseComponent("Transform Component")
			,m_position(_other.m_position)
			,m_rotation(_other.m_rotation)
			,m_scale(_other.m_scale)
		{

		}

		float GetPosX() const
		{
			return m_position.x;
		}

		float GetPosY() const
		{
			return m_position.y;
		}

		float GetPosZ() const
		{
			return m_position.z;
		}

		float GetRotPitch() const
		{
			return m_rotation.x;
		}

		float GetRotYaw() const
		{
			return m_rotation.y;
		}

		float GetRotZ() const
		{
			return m_rotation.z;
		}

		float GetScaleX() const
		{
			return m_scale.x;
		}

		float GetScaleY() const
		{
			return m_scale.y;
		}

		float GetScaleZ() const
		{
			return m_scale.z;
		}

		DirectX::XMFLOAT3 GetPos() const
		{
			return m_position;
		}

		DirectX::XMFLOAT3 GetRot() const
		{
			return m_rotation;
		}

		DirectX::XMFLOAT3 GetScale() const
		{
			return m_scale;
		}

		void SetPos(const DirectX::XMFLOAT3& _pos)
		{
			m_position = _pos;
		}

		void SetRot(const DirectX::XMFLOAT3& _rot)
		{
			m_rotation = _rot;
		}

		void SetScale(const DirectX::XMFLOAT3& _scale)
		{
			m_scale = _scale;
		}

		virtual void OnSave() override
		{
			Json posJson = 
			{
				{JSONPOSX, m_position.x},
				{JSONPOSY, m_position.y},
				{JSONPOSZ, m_position.z}
			};
			Json rotJson = 
			{
				{JSONROTPITCH, m_rotation.x},
				{JSONROTYAW, m_rotation.y},
				{JSONROTROLL, m_rotation.z}
			};
			Json scaleJson = 
			{
				{JSONSCALEX, m_scale.x},
				{JSONSCALEY, m_scale.y},
				{JSONSCALEZ, m_scale.z}
			};

			m_jsonObject[JSONPOS] = posJson;
			m_jsonObject[JSONROT] = rotJson;
			m_jsonObject[JSONSCALE] = scaleJson;
		}

	private:
		DirectX::XMFLOAT3 m_position;
		DirectX::XMFLOAT3 m_rotation;
		DirectX::XMFLOAT3 m_scale;
	};
}