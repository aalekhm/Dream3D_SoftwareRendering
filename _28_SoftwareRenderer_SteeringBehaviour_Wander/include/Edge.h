#pragma once
#include <cstdint>
#include "Vertex.h"
#include "Gradients.h"

class Edge
{
	public:
						Edge(Gradients gradients, Vertex vStart, Vertex vEnd, int32_t iMinYVertIndex);

		int32_t			GetYStart();
		int32_t			GetYEnd();

		float			GetX();
		float			GetTexCoordX();
		float			GetTexCoordY();
		float			GetOneOverZ();
		float			GetDepth();
		float			GetLightAmt();
		float			GetColourR();
		float			GetColourG();
		float			GetColourB();
		float			GetColourA();

		void			Step();
	protected:
	private:
		int32_t			m_iYStart;
		int32_t			m_iYEnd;

		float			m_fX;
		float			m_fXStep;
		float			m_fTexCoordX;
		float			m_fTexCoordXStep;
		float			m_fTexCoordY;
		float			m_fTexCoordYStep;
		float			m_fOneOverZ;
		float			m_fOneOverZStep;
		float			m_fDepth;
		float			m_fDepthStep;
		float			m_fLightAmt;
		float			m_fLightAmtStep;
		float			m_fColourR;
		float			m_fColourRStep;
		float			m_fColourG;
		float			m_fColourGStep;
		float			m_fColourB;
		float			m_fColourBStep;
		float			m_fColourA;
		float			m_fColourAStep;
};