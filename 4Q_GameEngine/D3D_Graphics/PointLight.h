#pragma once
class PointLight
{
public:
	void SetPosition(Math::Vector3 pos) { m_position = pos; }
	Math::Vector3 GetPosition() { return m_position; }

	void SetColor(float r = 1.0f, float g = 1.0f, float b = 1.0f) { m_color = Math::Vector3(r, g, b); }
	void SetColor(Vector3 color) { m_color = color; }
	Math::Vector3 GetColor() { return m_color; }

	void SetRadius(float rad) { m_radius = rad; }
	float GetRadius() { return m_radius; }

	void SetIntensity(float intensity) { m_intensity = intensity; }
	float GetIntensity() { return m_intensity; }

	float GetConstantTerm() { return m_constantTerm;  }

	void SetID(int id) { m_entID = id; }
	int GetID() { return m_entID; }
private:
	int m_entID = 0;
	Math::Vector3 m_position;
	Math::Vector3 m_color;
	float m_radius;
	float m_intensity;
	float m_constantTerm = 1.0f;
};

