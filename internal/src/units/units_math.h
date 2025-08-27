#pragma once
#include <cmath>
#include "./units_macros.h"
#include "./units_types.h"

namespace Units {
	namespace Math {
		/* ### \- Units Vector2 class: 
			- can be declared constexpr */
		template<typename T>
		struct Vector2;
		/* ### \- Units Vector3 class: 
			- can be declared constexpr */
		template<typename T>
		struct Vector3;


		template<typename T>
		struct Vector2 {
			T x, y;

			UNITS_NODISCARD constexpr Vector2() noexcept : x( 0 ), y( 0 ) {}
			template<typename t>
			UNITS_NODISCARD constexpr Vector2( t n ) noexcept : x( n ), y( n ) {}
			template<typename t, typename u>
			UNITS_NODISCARD constexpr Vector2( t _x, u _y ) noexcept : x( _x ), y( _y ) {}
			template<typename t>
			UNITS_NODISCARD constexpr Vector2( const Vector2<t> &_vec ) noexcept : x( _vec.x ), y( _vec.y ) {}
			template<typename t>
			UNITS_NODISCARD constexpr Vector2( const Vector3<t> &_vec ) noexcept : x( _vec.x ), y( _vec.y ) {}

			template<typename t>
			UNITS_NODISCARD bool operator==( const Vector2<t> &_vec ) const noexcept { return x == _vec.x && y == _vec.y; }
			template<typename t>
			UNITS_NODISCARD bool operator==( const Vector3<t> &_vec ) noexcept { return x == _vec.x && y == _vec.y; }
			template<typename t>
			UNITS_NODISCARD bool operator!=( const Vector2<t> &_vec ) noexcept { return x != _vec.x || y != _vec.y; }
			template<typename t>
			UNITS_NODISCARD bool operator!=( const Vector3<t> &_vec ) noexcept { return x != _vec.x || y != _vec.y; }

			template<typename t>
			UNITS_NODISCARD Vector2<t> operator+( const t& n ) const noexcept { return Vector2<t>{ x + n, y + n }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator-( const t& n ) const noexcept { return Vector2<t>{ x - n, y - n }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator*( const t& n ) const noexcept { return Vector2<t>{ x * n, y * n }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator/( const t& n ) const noexcept { return Vector2<t>{ x / n, y / n }; }

			template<typename t>
			UNITS_NODISCARD Vector2<t> operator+( const Vector2<t> &_vec ) noexcept { return Vector2<t>{ x + _vec.x, y + _vec.y }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator-( const Vector2<t> &_vec ) noexcept { return Vector2<t>{ x - _vec.x, y - _vec.y }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator*( const Vector2<t> &_vec ) noexcept { return Vector2<t>{ x * _vec.x, y * _vec.y }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator/( const Vector2<t> &_vec ) noexcept { return Vector2<t>{ x / _vec.x, y / _vec.y }; }

			template<typename t>
			UNITS_NODISCARD Vector2<t> operator+( const Vector3<t> &_vec ) noexcept { return Vector2<t>{ x + _vec.x, y + _vec.y }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator-( const Vector3<t> &_vec ) noexcept { return Vector2<t>{ x - _vec.x, y - _vec.y }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator*( const Vector3<t> &_vec ) noexcept { return Vector2<t>{ x * _vec.x, y * _vec.y }; }
			template<typename t>
			UNITS_NODISCARD Vector2<t> operator/( const Vector3<t> &_vec ) noexcept { return Vector2<t>{ x / _vec.x, y / _vec.y }; }

			template<typename t>
			void operator+=( const t& n ) noexcept { x += n; y += n; }
			template<typename t>
			void operator-=( const t& n ) noexcept { x -= n; y -= n; }
			template<typename t>
			void operator*=( const t& n ) noexcept { x *= n; y *= n; }
			template<typename t>
			void operator/=( const t& n ) noexcept { x /= n; y /= n; }

			template<typename t>
			void operator+=( const Vector2<t> &_vec ) noexcept { x += _vec.x; y += _vec.y; }
			template<typename t>
			void operator-=( const Vector2<t> &_vec ) noexcept { x -= _vec.x; y -= _vec.y; }
			template<typename t>
			void operator*=( const Vector2<t> &_vec ) noexcept { x *= _vec.x; y *= _vec.y; }
			template<typename t>
			void operator/=( const Vector2<t> &_vec ) noexcept { x /= _vec.x; y /= _vec.y; }

			template<typename t>
			void operator+=( const Vector3<t> &_vec ) noexcept { x += _vec.x; y += _vec.y; }
			template<typename t>
			void operator-=( const Vector3<t> &_vec ) noexcept { x -= _vec.x; y -= _vec.y; }
			template<typename t>
			void operator*=( const Vector3<t> &_vec ) noexcept { x *= _vec.x; y *= _vec.y; }
			template<typename t>
			void operator/=( const Vector3<t> &_vec ) noexcept { x /= _vec.x; y /= _vec.y; }

			void clear() noexcept { x = 0; y = 0; }
			void invert() noexcept { x = -x; y = -y; }
			template<typename t = float64_t>
			UNITS_NODISCARD t magnitude() const noexcept { return std::hypot( t( x ), t( y ) ); }
			void normalize() noexcept { double mag = magnitude(); x /= mag; y /= mag; }
			template<typename t = T>
			UNITS_NODISCARD Vector2<t> normal() const noexcept { double mag = magnitude(); return Vector2<t>{ x / mag, y / mag }; }

			static const Vector2<T> Zero;
			static const Vector2<T> One;
			static const Vector2<T> Up;
			static const Vector2<T> Down;
			static const Vector2<T> Left;
			static const Vector2<T> Right;
		};

		template<typename T>
		const Vector2<T> Vector2<T>::Zero (  0,  0 );
		template<typename T>
		const Vector2<T> Vector2<T>::One  (  1,  1 );
		template<typename T>
		const Vector2<T> Vector2<T>::Up   (  0,  1 );
		template<typename T>
		const Vector2<T> Vector2<T>::Down (  0, -1 );
		template<typename T>
		const Vector2<T> Vector2<T>::Left ( -1,  0 );
		template<typename T>
		const Vector2<T> Vector2<T>::Right(  1,  0 );


		template<typename T>
		struct Vector3 {
			T x, y, z;

			UNITS_NODISCARD constexpr Vector3() noexcept : x( 0 ), y( 0 ), z( 0 ) {}
			template<typename t>
			UNITS_NODISCARD constexpr Vector3( t n ) noexcept : x( n ), y( n ), z( n ) {}
			template<typename t, typename u, typename v>
			UNITS_NODISCARD constexpr Vector3( t _x, u _y, v _z ) noexcept : x( _x ), y( _y ), z( _z ) {}
			template<typename t>
			UNITS_NODISCARD constexpr Vector3( const Vector2<t> &_vec ) noexcept : x( _vec.x ), y( _vec.y ), z( 0 ) {}
			template<typename t>
			UNITS_NODISCARD constexpr Vector3( const Vector3<t> &_vec ) noexcept : x( _vec.x ), y( _vec.y ), z( _vec.z ) {}

			template<typename t>
			UNITS_NODISCARD bool operator==( const Vector3<t> &_vec ) const noexcept { return x == _vec.x && y == _vec.y && z == _vec.z; }
			template<typename t>
			UNITS_NODISCARD bool operator==( const Vector2<t> &_vec ) const noexcept { return x == _vec.x && y == _vec.y; }
			template<typename t>
			UNITS_NODISCARD bool operator!=( const Vector3<t> &_vec ) const noexcept { return x != _vec.x || y != _vec.y || z != _vec.z; }
			template<typename t>
			UNITS_NODISCARD bool operator!=( const Vector2<t> &_vec ) const noexcept { return x != _vec.x || y != _vec.y; }

			template<typename t>
			UNITS_NODISCARD Vector3<t> operator+( const t& n ) const noexcept { return Vector3<t>{ x + n, y + n, z + n }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator-( const t& n ) const noexcept { return Vector3<t>{ x - n, y - n, z - n }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator*( const t& n ) const noexcept { return Vector3<t>{ x * n, y * n, z * n }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator/( const t& n ) const noexcept { return Vector3<t>{ x / n, y / n, z / n }; }

			template<typename t>
			UNITS_NODISCARD Vector3<t> operator+( const Vector3<t> &_vec ) const noexcept { return Vector3<t>{ x + _vec.x, y + _vec.y, z + _vec.z }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator-( const Vector3<t> &_vec ) const noexcept { return Vector3<t>{ x - _vec.x, y - _vec.y, z - _vec.z }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator*( const Vector3<t> &_vec ) const noexcept { return Vector3<t>{ x * _vec.x, y * _vec.y, z * _vec.z }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator/( const Vector3<t> &_vec ) const noexcept { return Vector3<t>{ x / _vec.x, y / _vec.y, z / _vec.z }; }

			template<typename t>
			UNITS_NODISCARD Vector3<t> operator+( const Vector2<t> &_vec ) const noexcept { return Vector3<t>{ x + _vec.x, y + _vec.y, z }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator-( const Vector2<t> &_vec ) const noexcept { return Vector3<t>{ x - _vec.x, y - _vec.y, z }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator*( const Vector2<t> &_vec ) const noexcept { return Vector3<t>{ x * _vec.x, y * _vec.y, z }; }
			template<typename t>
			UNITS_NODISCARD Vector3<t> operator/( const Vector2<t> &_vec ) const noexcept { return Vector3<t>{ x / _vec.x, y / _vec.y, z }; }

			template<typename t>
			void operator+=( const t& n ) noexcept { x += n; y += n; z += n; }
			template<typename t>
			void operator-=( const t& n ) noexcept { x -= n; y -= n; z -= n; }
			template<typename t>
			void operator*=( const t& n ) noexcept { x *= n; y *= n; z *= n; }
			template<typename t>
			void operator/=( const t& n ) noexcept { x /= n; y /= n; z /= n; }

			template<typename t>
			void operator+=( const Vector3<t> &_vec ) noexcept { x += _vec.x; y += _vec.y; z += _vec.z; }
			template<typename t>
			void operator-=( const Vector3<t> &_vec ) noexcept { x -= _vec.x; y -= _vec.y; z -= _vec.z; }
			template<typename t>
			void operator*=( const Vector3<t> &_vec ) noexcept { x *= _vec.x; y *= _vec.y; z *= _vec.z; }
			template<typename t>
			void operator/=( const Vector3<t> &_vec ) noexcept { x /= _vec.x; y /= _vec.y; z /= _vec.z; }

			template<typename t>
			void operator+=( const Vector2<t> &_vec ) noexcept { x += _vec.x; y += _vec.y; }
			template<typename t>
			void operator-=( const Vector2<t> &_vec ) noexcept { x -= _vec.x; y -= _vec.y; }
			template<typename t>
			void operator*=( const Vector2<t> &_vec ) noexcept { x *= _vec.x; y *= _vec.y; }
			template<typename t>
			void operator/=( const Vector2<t> &_vec ) noexcept { x /= _vec.x; y /= _vec.y; }

			void clear() noexcept { x = 0; y = 0; z = 0; }
			void invert() noexcept { x = -x; y = -y; z = -z; }
			template<typename t = float64_t>
			UNITS_NODISCARD t magnitude() const noexcept { return std::hypot( t( x ), t( y ), t( z ) ); }
			void normalize() noexcept { double mag = magnitude(); x /= mag; y /= mag; z /= mag; }
			template<typename t = T>
			UNITS_NODISCARD Vector3<t> normal() const noexcept { double mag = magnitude(); return Vector3<t>{ x / mag, y / mag, z / mag }; }

			static const Vector3<T> Zero;
			static const Vector3<T> One;
			static const Vector3<T> Up;
			static const Vector3<T> Down;
			static const Vector3<T> Left;
			static const Vector3<T> Right;
			static const Vector3<T> Forward;
			static const Vector3<T> Backward;
		};

		template<typename T>
		const Vector3<T> Vector3<T>::Zero    (  0,  0,  0 );
		template<typename T>
		const Vector3<T> Vector3<T>::One     (  1,  1,  1 );
		template<typename T>
		const Vector3<T> Vector3<T>::Up      (  0,  1,  0 );
		template<typename T>
		const Vector3<T> Vector3<T>::Down    (  0, -1,  0 );
		template<typename T>
		const Vector3<T> Vector3<T>::Left    ( -1,  0,  0 );
		template<typename T>
		const Vector3<T> Vector3<T>::Right   (  1,  0,  0 );
		template<typename T>
		const Vector3<T> Vector3<T>::Forward (  0,  0,  1 );
		template<typename T>
		const Vector3<T> Vector3<T>::Backward(  0,  0, -1 );
	} // namespace Math
} // namespace UnitsEngine

template<typename t>
UNITS_NODISCARD Units::Math::Vector2<t> operator+( const t& lhs, const Units::Math::Vector2<t>& rhs ) noexcept { return Units::Math::Vector2<t>{ lhs + rhs.x, lhs + rhs.y }; }
template<typename t>
UNITS_NODISCARD Units::Math::Vector2<t> operator-( const t& lhs, const Units::Math::Vector2<t>& rhs ) noexcept { return Units::Math::Vector2<t>{ lhs - rhs.x, lhs - rhs.y }; }
template<typename t>
UNITS_NODISCARD Units::Math::Vector2<t> operator*( const t& lhs, const Units::Math::Vector2<t>& rhs ) noexcept { return Units::Math::Vector2<t>{ lhs * rhs.x, lhs * rhs.y }; }
template<typename t>
UNITS_NODISCARD Units::Math::Vector2<t> operator/( const t& lhs, const Units::Math::Vector2<t>& rhs ) noexcept { return Units::Math::Vector2<t>{ lhs / rhs.x, lhs / rhs.y }; }

template<typename t>
UNITS_NODISCARD Units::Math::Vector3<t> operator+( const t& lhs, const Units::Math::Vector3<t>& rhs ) noexcept { return Units::Math::Vector3<t>{ lhs + rhs.x, lhs + rhs.y, lhs + rhs.z }; }
template<typename t>
UNITS_NODISCARD Units::Math::Vector3<t> operator-( const t& lhs, const Units::Math::Vector3<t>& rhs ) noexcept { return Units::Math::Vector3<t>{ lhs - rhs.x, lhs - rhs.y, lhs - rhs.z }; }
template<typename t>
UNITS_NODISCARD Units::Math::Vector3<t> operator*( const t& lhs, const Units::Math::Vector3<t>& rhs ) noexcept { return Units::Math::Vector3<t>{ lhs * rhs.x, lhs * rhs.y, lhs * rhs.z }; }
template<typename t>
UNITS_NODISCARD Units::Math::Vector3<t> operator/( const t& lhs, const Units::Math::Vector3<t>& rhs ) noexcept { return Units::Math::Vector3<t>{ lhs / rhs.x, lhs / rhs.y, lhs / rhs.z }; }