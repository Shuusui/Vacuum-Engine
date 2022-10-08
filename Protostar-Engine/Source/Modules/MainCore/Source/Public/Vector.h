#pragma once
#include "GlobalDefinitions.h"

namespace Protostar::Core
{
	template<typename TYPE, s8 DIMENSION>
	class Vector
	{
	protected:
		TYPE Get(s8 _dimensionValue) const
		{
			return m_params[_dimensionValue];
		}

		void Set(TYPE _value, s8 _dimensionValue)
		{
			m_params[_dimensionValue] = _value;
		}

		TYPE m_params[DIMENSION];
	};

	template<typename TYPE>
	class Vector2D : public Vector<TYPE, 2>
	{
	public:
		Vector2D()
		{
		}
		Vector2D(TYPE _value)
		{
			SetX(_value);
			SetY(_value);
		}
		Vector2D(TYPE _x, TYPE _y)
		{
			SetX(_x);
			SetY(_y);
		}
		TYPE GetX() const
		{
			return Vector<TYPE, 2>::Get(0);
		}

		TYPE GetY() const
		{
			return Vector<TYPE, 2>::Get(1);
		}

		void SetX(TYPE _value)
		{
			Vector<TYPE, 2>::Set(_value, 0);
		}

		void SetY(TYPE _value)
		{
			Vector<TYPE, 2>::Set(_value, 1);
		}
	};

	template<typename TYPE>
	class Vector3D : public Vector<TYPE, 3>
	{
	public:

		TYPE GetX() const
		{
			return Vector<TYPE, 3>::Get(0);
		}
		
		TYPE GetY() const
		{
			return Vector<TYPE, 3>::Get(1);
		}

		TYPE GetZ() const
		{
			return Vector<TYPE, 3>::Get(2);
		}

		void SetX(TYPE _value)
		{
			Vector<TYPE, 3>::Set(_value, 0);
		}

		void SetY(TYPE _value)
		{
			Vector<TYPE, 3>::Set(_value, 1);
		}

		void SetZ(TYPE _value)
		{
			Vector<TYPE, 3>::Set(_value, 2);
		}
	};

	template<typename TYPE>
	class Vector4D : public Vector<TYPE, 4>
	{
	public:
		TYPE GetX() const
		{
			return Vector<TYPE, 4>::Get(0);
		}

		TYPE GetY() const 
		{
			return Vector<TYPE, 4>::Get(1);
		}

		TYPE GetZ() const
		{
			return Vector<TYPE, 4>::Get(2);
		}

		TYPE GetW() const
		{
			return Vector<TYPE, 4>::Get(3);
		}

		void SetX(TYPE _value)
		{
			Vector<TYPE, 4>::Set(_value, 0);
		}
		
		void SetY(TYPE _value)
		{
			Vector<TYPE, 4>::Set(_value, 1);
		}

		void SetZ(TYPE _value)
		{
			Vector<TYPE, 4>::Set(_value, 2);
		}

		void SetW(TYPE _value)
		{
			Vector<TYPE, 4>::Set(_value, 3);
		}
	};
}