#pragma once
#include "pch.h"
#include <cmath>

class Vector3D
{
public:
	Vector3D() = default;
	Vector3D(float _x, float _y, float _z)
		: m_X(_x)
		, m_Y(_y)
		, m_Z(_z)
	{}
private:
	float m_X = 0;
	float m_Y= 0;
	float m_Z = 0;

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
	void operator+=(const Vector3D& other)
	{
		this->m_X = m_X + other.m_X;
		this->m_Y = m_Y + other.m_Y;
		this->m_Z = m_Z + other.m_Z;
	}
	float Dot(const Vector3D& other) const {
		return m_X * other.m_X + m_Y * other.m_Y + m_Z * other.m_Z;
	}
	float Length() const
	{
		return sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z);
	}
	float GetX() const { return m_X; }
	float GetY() const { return m_Y; }
	float GetZ() const { return m_Z; }
};