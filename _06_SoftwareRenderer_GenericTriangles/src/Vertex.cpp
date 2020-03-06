#include "Vertex.h"

Vertex::Vertex(float fX, float fY)
: m_fX(fX)
, m_fY(fY)
{
}

float Vertex::GetX()
{
	return m_fX;
}

float Vertex::GetY()
{
	return m_fY;
}

void Vertex::SetX(float fX)
{
	m_fX = fX;
}

void Vertex::SetY(float fY)
{
	m_fY = fY;
}

float Vertex::TriangleArea(Vertex b, Vertex c)
{
	float x1 = b.GetX() - m_fX;
	float y1 = b.GetY() - m_fY;

	float x2 = c.GetX() - m_fX;
	float y2 = c.GetY() - m_fY;

	return (x1 * y2 - x2 * y1);
}