#include "Gradients.h"

Gradients::Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert)
{
	m_vColour[0] = vMinYVert.GetColour();
	m_vColour[1] = vMidYVert.GetColour();
	m_vColour[2] = vMaxYVert.GetColour();

	float _1OverDx = 1.0f / (
								(
									(vMidYVert.GetX() - vMaxYVert.GetX()) *
									(vMinYVert.GetY() - vMaxYVert.GetY())
								) -
								(	
									(vMinYVert.GetX() - vMaxYVert.GetX()) *
									(vMidYVert.GetY() - vMaxYVert.GetY())
								)
							);

		float oneOverdY = -_1OverDx;

		m_vColourXStep = ((	(m_vColour[1].sub(m_vColour[2])).mul(
							(vMinYVert.GetY() - vMaxYVert.GetY()))).sub(
							((m_vColour[0].sub(m_vColour[2])).mul(
							(vMidYVert.GetY() - vMaxYVert.GetY()))))).mul(_1OverDx);

		m_vColourYStep = (((m_vColour[1].sub(m_vColour[2])).mul(
							(vMinYVert.GetX() - vMaxYVert.GetX()))).sub(
							((m_vColour[0].sub(m_vColour[2])).mul(
							(vMidYVert.GetX() - vMaxYVert.GetX()))))).mul(oneOverdY);
}

Vector4f Gradients::GetColour(int32_t iLoc)
{
	return m_vColour[iLoc];
}

Vector4f Gradients::GetColourXStep()
{
	return m_vColourXStep;
}

Vector4f Gradients::GetColourYStep()
{
	return m_vColourYStep;
}
