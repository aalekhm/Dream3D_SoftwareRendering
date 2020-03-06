#include "Gradients.h"

Gradients::Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert)
{
	// Note that the 'W' component is the perspective 'Z' value;
	// The 'Z' component is the Occlusion 'Z' value.
	m_fOneOverZ[0] = 1.0f / vMinYVert.GetPosition().getW();
	m_fOneOverZ[1] = 1.0f / vMidYVert.GetPosition().getW();
	m_fOneOverZ[2] = 1.0f / vMaxYVert.GetPosition().getW();

	m_fTexCoordX[0] = vMinYVert.GetTexCoords().getX() * m_fOneOverZ[0];
	m_fTexCoordX[1] = vMidYVert.GetTexCoords().getX() * m_fOneOverZ[1];
	m_fTexCoordX[2] = vMaxYVert.GetTexCoords().getX() * m_fOneOverZ[2];

	m_fTexCoordY[0] = vMinYVert.GetTexCoords().getY() * m_fOneOverZ[0];
	m_fTexCoordY[1] = vMidYVert.GetTexCoords().getY() * m_fOneOverZ[1];
	m_fTexCoordY[2] = vMaxYVert.GetTexCoords().getY() * m_fOneOverZ[2];
	
	float fOneOverdX = 1.0f / (
								(	(vMidYVert.GetX() - vMaxYVert.GetX()) *
									(vMinYVert.GetY() - vMaxYVert.GetY())
								) -
								(	(vMinYVert.GetX() - vMaxYVert.GetX()) *
									(vMidYVert.GetY() - vMaxYVert.GetY())
								));

	float fOneOverdY = -fOneOverdX;

	m_fTexCoordXXStep = CalcXStep(m_fTexCoordX, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fTexCoordXYStep = CalcYStep(m_fTexCoordX, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fTexCoordYXStep = CalcXStep(m_fTexCoordY, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fTexCoordYYStep = CalcYStep(m_fTexCoordY, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);

	m_fOneOverZXStep = CalcXStep(m_fOneOverZ, vMinYVert, vMidYVert, vMaxYVert, fOneOverdX);
	m_fOneOverZYStep = CalcYStep(m_fOneOverZ, vMinYVert, vMidYVert, vMaxYVert, fOneOverdY);
}

float Gradients::CalcXStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDX)
{
	return	((	(pValues[1] - pValues[2])
				*
				(vMinYVert.GetY() - vMaxYVert.GetY())
			) -
			(	(pValues[0] - pValues[2])
				*
				(vMidYVert.GetY() - vMaxYVert.GetY())
			)) * fOneOverDX;
}

float Gradients::CalcYStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDY)
{
	return	((	(pValues[1] - pValues[2])
				*
				(vMinYVert.GetX() - vMaxYVert.GetX())
			) -
			(	(pValues[0] - pValues[2])
				*
				(vMidYVert.GetX() - vMaxYVert.GetX())
			)) * fOneOverDY;
}

float Gradients::GetTexCoordX(int32_t iLoc)
{
	return m_fTexCoordX[iLoc];
}

float Gradients::GetTexCoordY(int32_t iLoc)
{
	return m_fTexCoordY[iLoc];
}

float Gradients::GetOneOverZ(int32_t iLoc)
{
	return m_fOneOverZ[iLoc];
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

float Gradients::GetOneOverZXStep()
{
	return m_fOneOverZXStep;
}

float Gradients::GetOneOverZYStep()
{
	return m_fOneOverZYStep;
}
