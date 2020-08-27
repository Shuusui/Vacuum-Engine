#pragma once
#include "GlobalDefs.h"
#include <utility>
#include <vector>
#include "SharedEnums.h"
#include "DirectXMath.h"

namespace Vacuum
{
	struct SRendererCreationInfo
	{
		ERenderAPIs RenderApi;
		uint32 Width;
		uint32 Height;
		bool bVSync;
		void* WndHandle;
	};

	struct SVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT2 textureCoordinate;
	};

	struct SMesh
	{
		SMesh()
			:Vertices({})
			,Indices({})
		{

		}

		SMesh(const SMesh& _other)
			:Vertices(_other.Vertices)
			,Indices(_other.Indices)
		{

		}

		std::vector<SVertex> Vertices;
		std::vector<uint32> Indices;
	};

	struct SWindowDimParams
	{
		SWindowDimParams() = default;

		SWindowDimParams(const SWindowDimParams& _other)
			:Width(_other.Width)
			, Height(_other.Height)
			, LeftTopCornerX(_other.LeftTopCornerX)
			, LeftTopCornerY(_other.LeftTopCornerY)
		{
		}

		SWindowDimParams(SWindowDimParams&& _other) noexcept
			:Width(std::move(_other.Width))
			, Height(std::move(_other.Height))
			, LeftTopCornerX(std::move(_other.LeftTopCornerX))
			, LeftTopCornerY(std::move(_other.LeftTopCornerY))
		{
		}

		void operator=(const SWindowDimParams& _other)
		{
			Width = _other.Width;
			Height = _other.Height;
			LeftTopCornerX = _other.LeftTopCornerX;
			LeftTopCornerY = _other.LeftTopCornerY;
		}

		int64 Width;
		int64 Height;
		int32 LeftTopCornerX;
		int32 LeftTopCornerY;
	};
}