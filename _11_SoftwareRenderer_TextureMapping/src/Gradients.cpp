#include "Gradients.h"

Gradients::Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert)
{
	m_fTexCoordX[0] = vMinYVert.GetTexCoords().getX();
	m_fTexCoordX[1] = vMidYVert.GetTexCoords().getX();
	m_fTexCoordX[2] = vMaxYVert.GetTexCoords().getX();

	m_fTexCoordY[0] = vMinYVert.GetTexCoords().getY();
	m_fTexCoordY[1] = vMidYVert.GetTexCoords().getY();
	m_fTexCoordY[2] = vMaxYVert.GetTexCoords().getY();

	float oneOverdX = 1.0f / (
								(	(vMidYVert.GetX() - vMaxYVert.GetX()) *
									(vMinYVert.GetY() - vMaxYVert.GetY())
								) -
								(	(vMinYVert.GetX() - vMaxYVert.GetX()) *
									(vMidYVert.GetY() - vMaxYVert.GetY())
								));

	float oneOverdY = -oneOverdX;

	m_fTexCoordXXStep = 	((	(m_fTexCoordX[1] - m_fTexCoordX[2]) 
								*
								(vMinYVert.GetY() - vMaxYVert.GetY())
							) -
							(	(m_fTexCoordX[0] - m_fTexCoordX[2]) 
								*
								(vMidYVert.GetY() - vMaxYVert.GetY())
							)) * oneOverdX;

	m_fTexCoordXYStep = 	((	(m_fTexCoordX[1] - m_fTexCoordX[2]) 
								*
								(vMinYVert.GetX() - vMaxYVert.GetX())
							) -
							(	(m_fTexCoordX[0] - m_fTexCoordX[2]) 
								*
								(vMidYVert.GetX() - vMaxYVert.GetX())
							)) * oneOverdY;

	m_fTexCoordYXStep = 	((	(m_fTexCoordY[1] - m_fTexCoordY[2]) 
								*
								(vMinYVert.GetY() - vMaxYVert.GetY())
							) -
							(	(m_fTexCoordY[0] - m_fTexCoordY[2]) 
								*
								(vMidYVert.GetY() - vMaxYVert.GetY())
							)) * oneOverdX;

	m_fTexCoordYYStep = 	((	(m_fTexCoordY[1] - m_fTexCoordY[2]) 
								*
								(vMinYVert.GetX() - vMaxYVert.GetX())
							) -
							(	(m_fTexCoordY[0] - m_fTexCoordY[2]) 
								*
								(vMidYVert.GetX() - vMaxYVert.GetX())
							)) * oneOverdY;
}

float Gradients::GetTexCoordX(int32_t iLoc)
{
	return m_fTexCoordX[iLoc];
}

float Gradients::GetTexCoordY(int32_t iLoc)
{
	return m_fTexCoordY[iLoc];
}

float Gradients::GetTexCoordXXStep()
{
	return m_fTexCoordXXStep;
}

float Gradients::GetTexCoordXYStep()
{
	return m_fTexCoordXYStep;
}

float Gradients::GetTexCoordYXStep()
{
	return m_fTexCoordYXStep;
}

float Gradients::GetTexCoordYYStep()
{
	return m_fTexCoordYYStep;
}
