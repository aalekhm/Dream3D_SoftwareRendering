#pragma once
#include "Vector4f.h"
#include "Vertex.h"

class Gradients
{
	public:
						Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fGlobalAmbientLightIntensity);

		float			GetTexCoordX(int32_t iLoc);
		float			GetTexCoordY(int32_t iLoc);
		float			GetOneOverZ(int32_t iLoc);
		float			GetDepth(int32_t iLoc);
		float			GetLightAmt(int32_t iLoc);
		float			GetColourR(int32_t iLoc);
		float			GetColourG(int32_t iLoc);
		float			GetColourB(int32_t iLoc);
		float			GetColourA(int32_t iLoc);

		float			GetTexCoordXXStep();
		float			GetTexCoordXYStep();
		
		float			GetTexCoordYXStep();
		float			GetTexCoordYYStep();

		float			GetOneOverZXStep();
		float			GetOneOverZYStep();

		float			GetDepthXStep();
		float			GetDepthYStep();

		float			GetLightAmtXStep();
		float			GetLightAmtYStep();

		float			GetColourRXStep();
		float			GetColourRYStep();
		float			GetColourGXStep();
		float			GetColourGYStep();
		float			GetColourBXStep();
		float			GetColourBYStep();
		float			GetColourAXStep();
		float			GetColourAYStep();
	protected:
		float			CalcXStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDX);
		float			CalcYStep(float* pValues, Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, float fOneOverDY);
		float			Saturate(float fVal);
	private:
		float			m_fTexCoordX[3];
		float			m_fTexCoordY[3];
		float			m_fOneOverZ[3];
		float			m_fDepth[3];
		float			m_fLightAmt[3];
		float			m_fColourR[3];
		float			m_fColourG[3];
		float			m_fColourB[3];
		float			m_fColourA[3];

		float			m_fTexCoordXXStep;
		float			m_fTexCoordXYStep;
		float			m_fTexCoordYXStep;
		float			m_fTexCoordYYStep;
		float			m_fOneOverZXStep;
		float			m_fOneOverZYStep;
		float			m_fDepthXStep;
		float			m_fDepthYStep;
		float			m_fLightAmtXStep;
		float			m_fLightAmtYStep;
		float			m_fColourRXStep;
		float			m_fColourRYStep;
		float			m_fColourGXStep;
		float			m_fColourGYStep;
		float			m_fColourBXStep;
		float			m_fColourBYStep;
		float			m_fColourAXStep;
		float			m_fColourAYStep;
};