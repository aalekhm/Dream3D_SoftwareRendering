#pragma once
#include "Vector4f.h"
#include "Matrix4f.h"

class Vertex
{
	public:
						Vertex();
						Vertex(float fX, float fY, float fZ);
						Vertex(Vector4f vPos);

		float			GetX();
		float			GetY();
		float			GetZ();

		void			SetX(float fX);
		void			SetY(float fY);
		void			SetZ(float fZ);

		float			TriangleAreaTimesTwo(Vertex b, Vertex c);

		Vertex			Transform(Matrix4f transform);
		Vertex			PerspectiveDivide();
	protected:
	private:
		Vector4f		m_vPos;
};