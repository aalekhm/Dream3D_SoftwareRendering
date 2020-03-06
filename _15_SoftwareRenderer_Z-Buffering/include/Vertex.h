#pragma once
#include "Vector4f.h"
#include "Matrix4f.h"

class Vertex
{
	public:
						Vertex();
						Vertex(Vector4f vPos, Vector4f vColour);
						Vertex(Vector4f vPos);

		float			GetX();
		float			GetY();
		float			GetZ();

		Vector4f		GetPosition();
		Vector4f		GetTexCoords();

		void			SetX(float fX);
		void			SetY(float fY);
		void			SetZ(float fZ);

		float			TriangleAreaTimesTwo(Vertex b, Vertex c);

		Vertex			Transform(Matrix4f transform);
		Vertex			PerspectiveDivide();

		std::string		ToString();
	protected:
	private:
		Vector4f		m_vPos;
		Vector4f		m_vTexCoords;
};