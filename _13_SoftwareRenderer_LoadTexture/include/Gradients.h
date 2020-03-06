#pragma once
#include "Vector4f.h"
#include "Vertex.h"

class Gradients
{
	public:
						Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert);

		float			GetTexCoordX(int32_t iLoc);
		float			GetTexCoordY(int32_t iLoc);
		float			GetOneOverZ(int32_t iLoc);

		float			GetTexCoordXXStep();
		float			GetTexCoordXYStep();
		
		float			GetTexCoordYXStep();
		float			GetTexCoordYYStep();

		float			GetOneOverZXStep();
		float			GetOneOverZYStep();
	protected:
		float			CalcXStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDX);
		float			CalcYStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDY);
	private:
		float			m_fTexCoordX[3];
		float			m_fTexCoordY[3];
		float			m_fOneOverZ[3];

		float			m_fTexCoordXXStep;
		float			m_fTexCoordXYStep;
		float			m_fTexCoordYXStep;
		float			m_fTexCoordYYStep;
		float			m_fOneOverZXStep;
		float			m_fOneOverZYStep;
};