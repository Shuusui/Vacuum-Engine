#pragma once
#include "GlobalDefs.h"
#include <utility>
#include "SharedEnums.h"

namespace Vacuum
{
	struct SRendererCreationInfo
	{
		ERenderAPIs m_renderApi;
		uint32 m_width;
		uint32 m_height;
		void* m_wndHandle;
	};

	struct SWindowDimParams
	{
		SWindowDimParams() = default;

		SWindowDimParams(const SWindowDimParams& _other)
			:m_width(_other.m_width)
			, m_height(_other.m_height)
			, m_leftTopCornerX(_other.m_leftTopCornerX)
			, m_leftTopCornerY(_other.m_leftTopCornerY)
		{
		}

		SWindowDimParams(SWindowDimParams&& _other) noexcept
			:m_width(std::move(_other.m_width))
			, m_height(std::move(_other.m_height))
			, m_leftTopCornerX(std::move(_other.m_leftTopCornerX))
			, m_leftTopCornerY(std::move(_other.m_leftTopCornerY))
		{
		}

		void operator=(const SWindowDimParams& _other)
		{
			m_width = _other.m_width;
			m_height = _other.m_height;
			m_leftTopCornerX = _other.m_leftTopCornerX;
			m_leftTopCornerY = _other.m_leftTopCornerY;
		}

		int64 m_width;
		int64 m_height;
		int32 m_leftTopCornerX;
		int32 m_leftTopCornerY;
	};
}