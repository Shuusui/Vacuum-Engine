#include <array>
#include <cstddef>

namespace Protostar::Core
{
	template<typename T, std::size_t SIZE>
	class FixedSizeArray<T, SIZE, void>
	{
	public:
		[[nodiscard]] constexpr T& At(std::size_t _pos)
		{
			return m_container.at(_pos);
		}
		[[nodiscard]] constexpr const T& At(std::size_t _pos) const
		{
			return m_container.at(_pos);
		}
		[[nodiscard]] constexpr T& operator[](std::size_t _pos)
		{
			return m_container[_pos];
		}
		[[nodiscard]] constexpr const T& operator[](std::size_t _pos) const
		{
			return m_container[_pos];
		}
		[[nodiscard]] constexpr T& GetFirst()
		{
			return m_container.front();
		}
		[[nodiscard]] constexpr const T& GetFirst() const
		{
			return m_container.front();
		}
		[[nodiscard]] constexpr T& GetLast()
		{
			return m_container.back();
		}
		[[nodiscard]] constexpr const T& GetLast() const
		{
			return m_container.back();
		}
		[[nodiscard]] constexpr T* GetData() noexcept
		{
			return m_container.data();
		}
		[[nodiscard]] constexpr const T* GetData() const noexcept
		{
			return m_container.data();
		}
		[[nodiscard]] constexpr bool IsEmpty() const noexcept
		{
			return m_container.empty();
		}
		[[nodiscard]] constexpr std::size_t GetSize() const noexcept
		{
			return m_container.size();
		}
		[[nodiscard]] constexpr std::size_t GetMaxSize() const noexcept
		{
			return m_container.max_size();
		}
		constexpr void Fill(const T& _value)
		{
			m_container.fill(_value);
		}
		constexpr void Swap(FixedSizeArray& _other) noexcept
		{
			m_container.swap(_other.m_container);
		}
	private:

		std::array<T, SIZE> m_container;
	};
}