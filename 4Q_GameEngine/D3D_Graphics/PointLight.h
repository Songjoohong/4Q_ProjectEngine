#pragma once
class PointLight
{
public:
	void SetPosition(Math::Vector3 pos) { m_position = pos; }
	Math::Vector3 GetPosition() { return m_position; }

	void SetColor(float r = 1.0f, float g = 1.0f, float b = 1.0f) { m_color = Math::Vector3(r, g, b); }
	Math::Vector3 GetColor() { return m_color; }

	void SetRadius(float rad) { m_radius = rad; }
	float GetRadius() { return m_radius; }
	Math::Vector3 m_position;
	float m_radius;
	Math::Vector3 m_color;
private:
};

