#pragma once
#include "BaseComponent.h"
#include "MathExt.h"
#include <DirectXMath.h>

#define JSONROW0 "row_0"
#define JSONROW1 "row_1"
#define JSONROW2 "row_2"
#define JSONROW3 "row_3"

#define JSON0 "0"
#define JSON1 "1"
#define JSON2 "2"
#define JSON3 "3"

namespace Protostar
{
	class CTransformComponent : public CBaseComponent
	{
	public:
		CTransformComponent()
			:CBaseComponent("Transform Component", Json())
			,m_transformationMatrix(DirectX::XMMatrixIdentity())
		{
		}

		CTransformComponent(const Json& _json)
			:CBaseComponent("Transform Component", _json)
			,m_transformationMatrix(DirectX::XMMATRIX(
			RowFromJson(_json[JSONROW0].get<Json>()), 
			RowFromJson(_json[JSONROW1].get<Json>()),
			RowFromJson(_json[JSONROW2].get<Json>()),
			RowFromJson(_json[JSONROW3].get<Json>())))
		{
		}

		CTransformComponent(const CTransformComponent& _other)
			:CBaseComponent("Transform Component", _other.m_jsonObject)
			,m_transformationMatrix(_other.m_transformationMatrix)
		{
		}

		float GetPosX() const
		{
			return m_transformationMatrix.r[3].m128_f32[0];
		}

		float GetPosY() const
		{
			return m_transformationMatrix.r[3].m128_f32[1];
		}

		float GetPosZ() const
		{
			return m_transformationMatrix.r[3].m128_f32[2];
		}

		float GetPitch() const
		{
			return -asin(m_transformationMatrix.r[2].m128_f32[0]);
		}

		float GetYaw() const
		{
			float pitch = GetPitch();
			return FloatCompare(pitch, DirectX::XM_PIDIV2) ?
			atan2(m_transformationMatrix.r[0].m128_f32[1], m_transformationMatrix.r[0].m128_f32[2]):
			FloatCompare(pitch, -DirectX::XM_PIDIV2) ?
			atan2(-m_transformationMatrix.r[0].m128_f32[1], -m_transformationMatrix.r[0].m128_f32[2]):
			atan2(m_transformationMatrix.r[2].m128_f32[1] / cos(pitch), m_transformationMatrix.r[2].m128_f32[2] / cos(pitch));
		}

		float GetRoll() const
		{
			float pitch = GetPitch();
			return FloatCompare(pitch, DirectX::XM_PIDIV2) ?
				0.0f :
				FloatCompare(pitch, -DirectX::XM_PIDIV2) ?
				0.0f :
				atan2(m_transformationMatrix.r[1].m128_f32[0] / cos(pitch), m_transformationMatrix.r[0].m128_f32[0] / cos(pitch));
		}

		float GetScaleX() const
		{
			float scaleX = 0.0f;
			DirectX::XMFLOAT3 scaleV(
				m_transformationMatrix.r[0].m128_f32[0],
				m_transformationMatrix.r[1].m128_f32[0],
				m_transformationMatrix.r[2].m128_f32[0]);
			DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&scaleV);
			DirectX::XMStoreFloat(&scaleX, v);
			return scaleX;
		}

		float GetScaleY() const
		{
			float scaleY = 0.0f;
			DirectX::XMFLOAT3 scaleV(
				m_transformationMatrix.r[0].m128_f32[1],
				m_transformationMatrix.r[1].m128_f32[1],
				m_transformationMatrix.r[2].m128_f32[1]);
			DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&scaleV);
			DirectX::XMStoreFloat(&scaleY, v);
			return scaleY;
		}

		float GetScaleZ() const
		{
			float scaleZ = 0.0f;
			DirectX::XMFLOAT3 scaleV(
				m_transformationMatrix.r[0].m128_f32[2],
				m_transformationMatrix.r[1].m128_f32[2],
				m_transformationMatrix.r[2].m128_f32[2]);
			DirectX::XMVECTOR v = DirectX::XMLoadFloat3(&scaleV);
			DirectX::XMStoreFloat(&scaleZ, v);
			return scaleZ;
		}

		DirectX::XMMATRIX GetTransformationMatrix() const
		{
			return m_transformationMatrix;
		}

		DirectX::XMFLOAT3 GetPos() const
		{
			return DirectX::XMFLOAT3(
				m_transformationMatrix.r[3].m128_f32[0],
				m_transformationMatrix.r[3].m128_f32[1],
				m_transformationMatrix.r[3].m128_f32[2]);
		}

		DirectX::XMFLOAT3 GetRot() const
		{
			return DirectX::XMFLOAT3(GetPitch(), GetYaw(), GetRoll());
		}

		DirectX::XMFLOAT3 GetScale() const
		{
			return DirectX::XMFLOAT3(
				GetScaleX(), 
				GetScaleY(), 
				GetScaleZ());
		}

		void SetPos(const DirectX::XMFLOAT3& _pos)
		{
			m_transformationMatrix.r[3].m128_f32[0] = _pos.x;
			m_transformationMatrix.r[3].m128_f32[1] = _pos.y;
			m_transformationMatrix.r[3].m128_f32[2] = _pos.z;
		}

		void SetPosX(const float& _posX)
		{
			m_transformationMatrix.r[3].m128_f32[0] = _posX;
		}

		void SetPosY(const float& _posY)
		{
			m_transformationMatrix.r[3].m128_f32[1] = _posY;
		}

		void SetPosZ(const float& _posZ)
		{
			m_transformationMatrix.r[3].m128_f32[2] = _posZ;
		}

		void SetRot(const DirectX::XMFLOAT3& _rot)
		{
			DirectX::XMMATRIX rotMatrix = DirectX::XMMatrixIdentity();

			rotMatrix.r[0].m128_f32[0] = cos(_rot.y) * cos(_rot.z);
			rotMatrix.r[0].m128_f32[1] = (sin(_rot.x) * sin(_rot.y) * cos(_rot.z)) - (cos(_rot.x) * sin(_rot.z));
			rotMatrix.r[0].m128_f32[2] = (cos(_rot.x) * sin(_rot.y) * cos(_rot.z)) + (sin(_rot.x) * sin(_rot.z));

			m_transformationMatrix = DirectX::XMMatrixMultiply(m_transformationMatrix, rotMatrix);
			rotMatrix = DirectX::XMMatrixIdentity();

			rotMatrix.r[1].m128_f32[0] = cos(_rot.y) * sin(_rot.z);
			rotMatrix.r[1].m128_f32[1] = (sin(_rot.x) * sin(_rot.y) * sin(_rot.z)) + (cos(_rot.x) * cos(_rot.z));
			rotMatrix.r[1].m128_f32[2] = (cos(_rot.x) * sin(_rot.y) * sin(_rot.z)) - (sin(_rot.x) * cos(_rot.z));

			m_transformationMatrix = DirectX::XMMatrixMultiply(m_transformationMatrix, rotMatrix);
			rotMatrix = DirectX::XMMatrixIdentity();

			rotMatrix.r[2].m128_f32[0] = -sin(_rot.y);
			rotMatrix.r[2].m128_f32[1] = sin(_rot.x) * cos(_rot.y);
			rotMatrix.r[2].m128_f32[2] = cos(_rot.x) * cos(_rot.y);

			m_transformationMatrix = DirectX::XMMatrixMultiply(m_transformationMatrix, rotMatrix);
		}

		void SetScale(const DirectX::XMFLOAT3& _scale)
		{
			DirectX::XMMATRIX scaleMatrix = DirectX::XMMatrixIdentity();
			scaleMatrix.r[0].m128_f32[0] = _scale.x;
			scaleMatrix.r[1].m128_f32[1] = _scale.y;
			scaleMatrix.r[2].m128_f32[2] = _scale.z;

			m_transformationMatrix = DirectX::XMMatrixMultiply(m_transformationMatrix, scaleMatrix);
		}


		virtual void OnSave() override
		{
			m_jsonObject[JSONROW0] = RowToJson(m_transformationMatrix.r[0]);
			m_jsonObject[JSONROW1] = RowToJson(m_transformationMatrix.r[1]);
			m_jsonObject[JSONROW2] = RowToJson(m_transformationMatrix.r[2]);
			m_jsonObject[JSONROW3] = RowToJson(m_transformationMatrix.r[3]);
		}

	private:
		Json RowToJson(const DirectX::XMVECTOR& _row) const
		{
			return Json
			{
				{JSON0, _row.m128_f32[0]},
				{JSON1, _row.m128_f32[1]},
				{JSON2, _row.m128_f32[2]},
				{JSON3, _row.m128_f32[3]}
			};
		}

		DirectX::XMVECTOR RowFromJson(const Json& _json) const
		{
			return DirectX::XMVECTOR
			{
				_json[JSON0].get<float>(),
				_json[JSON1].get<float>(), 
				_json[JSON2].get<float>(),
				_json[JSON3].get<float>()
			};
		}

		DirectX::XMMATRIX m_transformationMatrix;
	};
}

#undef JSONROW0 
#undef JSONROW1 
#undef JSONROW2 
#undef JSONROW3 

#undef JSON0 
#undef JSON1 
#undef JSON2 
#undef JSON3 