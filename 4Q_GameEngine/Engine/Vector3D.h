#pragma once
#include "pch.h"
#include <cmath>
#include <directxtk/SimpleMath.h>

class Vector3D
{
public:
	Vector3D() = default;
	Vector3D(const float x, const float y, const float z)
		: m_X(x)
		, m_Y(y)
		, m_Z(z)
	{}
private:
	float m_X = 0.f;
	float m_Y= 0.f;
	float m_Z = 0.f;

public:
	Vector3D operator+(const Vector3D& other) const
	{
		return Vector3D{m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z};
	}

	Vector3D operator-(const Vector3D& other) const
	{
		return Vector3D{m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z};
	}

	Vector3D operator*(const float scalar) const
	{
		return Vector3D{m_X * scalar, m_Y * scalar, m_Z * scalar};
	}

	Vector3D operator/(const float scalar) const
	{
		if (scalar == 0.f)
			return *this;
		return Vector3D{ m_X / scalar, m_Y / scalar, m_Z / scalar };
	}

	DirectX::SimpleMath::Vector3 ConvertToVector3() const
	{
		return DirectX::SimpleMath::Vector3{ m_X, m_Y, m_Z };
	}

	void operator+=(const Vector3D& other)
	{
		this->m_X = m_X + other.m_X;
		this->m_Y = m_Y + other.m_Y;
		this->m_Z = m_Z + other.m_Z;
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
		if(Length() != 0)
			return Vector3D{ m_X / Length(), m_Y / Length(), m_Z / Length() };
		return *this;
	}

	float GetX() const { return m_X; }
	float GetY() const { return m_Y; }
	float GetZ() const { return m_Z; }
};
