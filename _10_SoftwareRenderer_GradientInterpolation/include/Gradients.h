#pragma once
#include "Vector4f.h"
#include "Vertex.h"

class Gradients
{
	public:
						Gradients(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert);

		Vector4f		GetColour(int32_t iLoc);
		Vector4f		GetColourXStep();
		Vector4f		GetColourYStep();
	protected:
	private:
		Vector4f		m_vColour[3];

		Vector4f		m_vColourXStep;
		Vector4f		m_vColourYStep;
};