#include "Edge.h"

Edge::Edge(Vertex vStart, Vertex vEnd)
{
	m_iYStart = (int32_t)ceil(vStart.GetY());
	m_iYEnd = (int32_t)ceil(vEnd.GetY());

	float fXDist = vEnd.GetX() - vStart.GetX();
	float fYDist = vEnd.GetY() - vStart.GetY();

	float fYPreStep = m_iYStart - vStart.GetY();

	m_fXStep = (float)fXDist / (float)fYDist;
	m_fX = vStart.GetX() + (fYPreStep * m_fXStep);
}

int32_t Edge::GetYStart()
{
	return m_iYStart;
}

int32_t Edge::GetYEnd()
{
	return m_iYEnd;
}

float Edge::GetX()
{
	return m_fX;
}

void Edge::Step()
{
	m_fX += m_fXStep;
}