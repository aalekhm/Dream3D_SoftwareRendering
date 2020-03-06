#include "Edge.h"

Edge::Edge(Gradients gradients, Vertex vStart, Vertex vEnd, int32_t iMinYVertIndex)
{
	m_iYStart = (int32_t)ceil(vStart.GetY());
	m_iYEnd = (int32_t)ceil(vEnd.GetY());

	float fXDist = vEnd.GetX() - vStart.GetX();
	float fYDist = vEnd.GetY() - vStart.GetY();

	float fYPreStep = m_iYStart - vStart.GetY();

	m_fXStep = (float)fXDist / (float)fYDist;
	m_fX = vStart.GetX() + (fYPreStep * m_fXStep);

	float fXPreStep = m_fX - vStart.GetX();

	m_vColour = gradients.GetColour(iMinYVertIndex).add(
				gradients.GetColourYStep().mul(fYPreStep)).add(
				gradients.GetColourXStep().mul(fXPreStep));
	m_vColourStep = gradients.GetColourYStep().add(gradients.GetColourXStep().mul(m_fXStep));
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

Vector4f Edge::GetColour()
{
	return m_vColour;
}

void Edge::Step()
{
	m_fX += m_fXStep;
	m_vColour = m_vColour.add(m_vColourStep);
}