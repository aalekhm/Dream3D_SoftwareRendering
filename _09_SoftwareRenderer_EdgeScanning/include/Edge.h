#pragma once
#include <cstdint>
#include "Vertex.h"

class Edge
{
	public:
						Edge(Vertex vStart, Vertex vEnd);
		void			Step();

		int32_t			GetYStart();
		int32_t			GetYEnd();

		float			GetX();
protected:

	private:
		float			m_fX;
		float			m_fXStep;

		int32_t			m_iYStart;
		int32_t			m_iYEnd;
};