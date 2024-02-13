#pragma once
#include "pch.h"
#include <cmath>
#include <DirectXMath.h>
#include <directxtk/SimpleMath.h>

using namespace DirectX::SimpleMath;
using namespace DirectX;
struct MyPoint
{
	LONG x;
	LONG y;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(MyPoint, x, y)
};
class Vector3D
{
public:
	Vector3D() = default;
	Vector3D(float _x, float _y, float _z)
		: m_X(_x)
		, m_Y(_y)
		, m_Z(_z)
	{}
	float m_X = 0;
	float m_Y = 0;
	float m_Z = 0;
private:

public:
	Vector3D operator+(const Vector3D& other) const
	{
		return Vector3D{ m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z };
	}

	Vector3D operator-(const Vector3D& other) const
	{
		return Vector3D{ m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z };
	}

	Vector3D operator*(const float scalar) const
	{
		return Vector3D{ m_X * scalar, m_Y * scalar, m_Z * scalar };
	}

	Vector3D operator/(const float scalar) const
	{
		if (scalar == 0.f)
			return *this;
		return Vector3D{ m_X / scalar, m_Y / scalar, m_Z / scalar };
	}

	Vector3 ConvertToVector3() const
	{
		return Vector3{ m_X, m_Y, m_Z };
	}


	void operator+=(const Vector3D& other)
	{
		this->m_X = m_X + other.m_X;
		this->m_Y = m_Y + other.m_Y;
		this->m_Z = m_Z + other.m_Z;
	}

	void operator+=(const Vector3& other)
	{
		this->m_X = m_X + other.x;
		this->m_Y = m_Y + other.y;
		this->m_Z = m_Z + other.z;
	}

	Vector3D& operator=(const DirectX::SimpleMath::Vector3& vec) 
	{
		m_X = vec.x;
		m_Y = vec.y;
		m_Z = vec.z;
		return *this;
	}
	Vector3D& operator=(const DirectX::XMFLOAT4& quaternion) {
		m_X = quaternion.x;
		m_Y = quaternion.y;
		m_Z = quaternion.z;
		return *this;
	}

	void operator-=(const Vector3D& other)
	{
		this->m_X = m_X - other.m_X;
		this->m_Y = m_Y - other.m_Y;
		this->m_Z = m_Z - other.m_Z;
	}

	float Dot(const Vector3D& other) const {
		return m_X * other.m_X + m_Y * other.m_Y + m_Z * other.m_Z;
	}

	Vector3D Cross(const Vector3D& other) const
	{
		return Vector3D{ m_Y * other.GetZ() - m_Z * other.GetY(), m_Z * other.GetX() - m_X * other.GetZ(), m_X * other.GetY() - m_Y * other.GetX() };
	}

	float Length() const
	{
		return sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z);
	}

	Vector3D Normalize() const
	{
		if (Length() != 0)
			return Vector3D{ m_X / Length(), m_Y / Length(), m_Z / Length() };
		return *this;
	}

	float GetX() const { return m_X; }
	float GetY() const { return m_Y; }
	float GetZ() const { return m_Z; }

	float& GetRefX() { return m_X; }
	float& GetRefY() { return m_Y; }
	float& GetRefZ() { return m_Z; }

	void SetX(float x) { m_X = x; }
	void SetY(float y) { m_Y = y; }
	void SetZ(float z) { m_Z = z; }


	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Vector3D, m_X, m_Y, m_Z)
};

