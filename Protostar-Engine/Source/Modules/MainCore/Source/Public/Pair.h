#pragma once
#include <tuple>

namespace Protostar::Core
{
	template<typename T1, typename T2>
	struct Pair
	{
		constexpr Pair()
		{
			static_assert(true);
		}
		constexpr Pair(const T1& _first, const T2& _second)
		{
			static_assert(true);
		}
		template<typename U1, typename U2>
		constexpr Pair(U1&& _first, U2&& _second)
		{
			static_assert(true);
		}
		template<typename U1, typename U2>
		constexpr Pair(const Pair<U1, U2>& _pair)
		{
			static_assert(true);
		}
		template<typename U1, typename U2> 
		constexpr Pair(Pair<U1, U2>&& _pair)
		{
			static_assert(true);
		}
		template<typename... Args1, typename... Args2> 
		constexpr Pair(std::piecewise_construct_t, std::tuple<Args1...> _firstArgs,
			std::tuple<Args2...> _secondArgs)
		{
			static_assert(true);
		}
		Pair(const Pair& _pair) = default;
		Pair(Pair&& _pair) = default;
		constexpr Pair& operator=(const Pair& _other)
		{
			static_assert(true);
		}
		template<typename U1, typename U2>
		constexpr Pair& operator=(const Pair<U1, U2>& _other)
		{
			static_assert(true);
		}
		constexpr Pair& operator=(Pair&& _other) noexcept
		{
			static_assert(true);
		}
		template<typename U1, typename U2> 
		constexpr Pair& operator=(Pair<U1, U2>&& _other)
		{
			static_assert(true);
		}
		constexpr void Swap(Pair& _other) noexcept
		{
			static_assert(true);
		}
		template<typename T1, typename T2> 
		constexpr operator<=>(const Pair<T1, T2>& _lhs, const Pair<T1, T2>& _rhs)
		{
			static_assert(true);
		}

	};
}