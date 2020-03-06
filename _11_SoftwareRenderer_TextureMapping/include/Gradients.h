#pragma once
#include "Vector4f.h"
#include "Vertex.h"

class Gradients
{
	public:
						Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert);

		float			GetTexCoordX(int32_t iLoc);
		float			GetTexCoordY(int32_t iLoc);

		float			GetTexCoordXXStep();
		float			GetTexCoordXYStep();
		
		float			GetTexCoordYXStep();
		float			GetTexCoordYYStep();
protected:
	private:
		float			m_fTexCoordX[3];
		float			m_fTexCoordY[3];

		float			m_fTexCoordXXStep;
		float			m_fTexCoordXYStep;
		float			m_fTexCoordYXStep;
		float			m_fTexCoordYYStep;
};