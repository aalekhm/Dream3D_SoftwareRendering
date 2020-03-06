#include "Edge.h"

Edge::Edge(Gradients pGradients, Vertex vStart, Vertex vEnd, int32_t iMinYVertIndex)
{
	m_iYStart = (int32_t)ceil(vStart.GetY());
	m_iYEnd = (int32_t)ceil(vEnd.GetY());

	float fXDist = vEnd.GetX() - vStart.GetX();
	float fYDist = vEnd.GetY() - vStart.GetY();

	float fYPreStep = m_iYStart - vStart.GetY();

	m_fXStep = (float)fXDist / (float)fYDist;
	m_fX = vStart.GetX() + (fYPreStep * m_fXStep);

	float fXPreStep = m_fX - vStart.GetX();

	m_fTexCoordX 	= 	pGradients.GetTexCoordX(iMinYVertIndex) +
						pGradients.GetTexCoordXXStep() * fXPreStep +
						pGradients.GetTexCoordXYStep() * fYPreStep;
	m_fTexCoordXStep = 	pGradients.GetTexCoordXYStep() + pGradients.GetTexCoordXXStep() * m_fXStep;

	m_fTexCoordY 	= 	pGradients.GetTexCoordY(iMinYVertIndex) +
						pGradients.GetTexCoordYXStep() * fXPreStep +
						pGradients.GetTexCoordYYStep() * fYPreStep;
	m_fTexCoordYStep = 	pGradients.GetTexCoordYYStep() + pGradients.GetTexCoordYXStep() * m_fXStep;

	m_fOneOverZ 	= 	pGradients.GetOneOverZ(iMinYVertIndex) +
						pGradients.GetOneOverZXStep() * fXPreStep +
						pGradients.GetOneOverZYStep() * fYPreStep;
	m_fOneOverZStep = 	pGradients.GetOneOverZYStep() + pGradients.GetOneOverZXStep() * m_fXStep;

	m_fDepth 		= 	pGradients.GetDepth(iMinYVertIndex) +
						pGradients.GetDepthXStep() * fXPreStep +
						pGradients.GetDepthYStep() * fYPreStep;
	m_fDepthStep	= 	pGradients.GetDepthYStep() + pGradients.GetDepthXStep() * m_fXStep;

	m_fLightAmt 	= 	pGradients.GetLightAmt(iMinYVertIndex) +
						pGradients.GetLightAmtXStep() * fXPreStep +
						pGradients.GetLightAmtYStep() * fYPreStep;
	m_fLightAmtStep	= 	pGradients.GetLightAmtYStep() + pGradients.GetLightAmtXStep() * m_fXStep;
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

float Edge::GetTexCoordX()
{
	return m_fTexCoordX;
}

float Edge::GetTexCoordY()
{
	return m_fTexCoordY;
}

float Edge::GetOneOverZ()
{
	return m_fOneOverZ;
}

float Edge::GetDepth()
{
	return m_fDepth;
}

float Edge::GetLightAmt()
{
	return m_fLightAmt;
}

void Edge::Step()
{
	m_fX			+= m_fXStep;
	m_fTexCoordX	+= m_fTexCoordXStep;
	m_fTexCoordY	+= m_fTexCoordYStep;
	m_fOneOverZ		+= m_fOneOverZStep;
	m_fDepth		+= m_fDepthStep;
	m_fLightAmt		+= m_fLightAmtStep;
}