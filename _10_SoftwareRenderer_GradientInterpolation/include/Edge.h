#pragma once
#include <cstdint>
#include "Vertex.h"
#include "Gradients.h"

class Edge
{
	public:
						Edge(Gradients gradients, Vertex vStart, Vertex vEnd, int32_t iMinYVertIndex);
		void			Step();

		int32_t			GetYStart();
		int32_t			GetYEnd();

		float			GetX();

		Vector4f		GetColour();
protected:

	private:
		float			m_fX;
		float			m_fXStep;

		int32_t			m_iYStart;
		int32_t			m_iYEnd;

		Vector4f		m_vColour;
		Vector4f		m_vColourStep;
};