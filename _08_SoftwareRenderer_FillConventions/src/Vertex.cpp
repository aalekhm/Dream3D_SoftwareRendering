#include "Vertex.h"

Vertex::Vertex()
{
	m_vPos.x = 0;
	m_vPos.y = 0;
	m_vPos.z = 0;
	m_vPos.w = 0;
}

Vertex::Vertex(float fX, float fY, float fZ)
{
	m_vPos.x = fX;
	m_vPos.y = fY;
	m_vPos.z = fZ;
	m_vPos.w = 1;
}

Vertex::Vertex(Vector4f vPos)
{
	m_vPos = vPos;
}

float Vertex::GetX()
{
	return m_vPos.getX();
}

float Vertex::GetY()
{
	return m_vPos.getY();
}

float Vertex::GetZ()
{
	return m_vPos.getZ();
}

void Vertex::SetX(float fX)
{
	m_vPos.x = fX;
}

void Vertex::SetY(float fY)
{
	m_vPos.y = fY;
}

void Vertex::SetZ(float fZ)
{
	m_vPos.z = fZ;
}

float Vertex::TriangleAreaTimesTwo(Vertex b, Vertex c)
{
	float x1 = b.GetX() - m_vPos.x;
	float y1 = b.GetY() - m_vPos.y;

	float x2 = c.GetX() - m_vPos.x;
	float y2 = c.GetY() - m_vPos.y;

	return (x1 * y2 - x2 * y1);
}

Vertex Vertex::Transform(Matrix4f transform)
{
	return Vertex( transform.transform(m_vPos) );
}

Vertex Vertex::PerspectiveDivide()
{
	return Vertex(Vector4f(	m_vPos.getX() / m_vPos.getW(),
							m_vPos.getY() / m_vPos.getW(),
							m_vPos.getZ() / m_vPos.getW(),
							m_vPos.getW()));
}
