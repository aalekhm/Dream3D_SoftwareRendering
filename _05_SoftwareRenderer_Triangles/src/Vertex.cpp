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
