#pragma once
#include "Vector4f.h"
#include "Matrix4f.h"

enum AXIS_COMPONENT
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_W
};

class Vertex
{
	public:
						Vertex();
						Vertex(Vector4f vPos, Vector4f vTexCoords, Vector4f vNormal, Vector4f vColour);
						Vertex(Vector4f vPos);

		float			GetX();
		float			GetY();
		float			GetZ();

		float			GetR();
		float			GetG();
		float			GetB();
		float			GetA();

		Vector4f		GetPosition();
		Vector4f		GetTexCoords();
		Vector4f		GetNormal();
		Vector4f		GetColour();

		void			SetX(float fX);
		void			SetY(float fY);
		void			SetZ(float fZ);

		void			SetR(float fX);
		void			SetG(float fY);
		void			SetB(float fZ);
		void			SetA(float fW);

		float			TriangleAreaTimesTwo(Vertex b, Vertex c);

		Vertex			Transform(Matrix4f transform);
		Vertex			Transform(Matrix4f transform, Matrix4f matNormalTransform);
		Vertex			PerspectiveDivide();

		std::string		ToString();

		Vertex			Lerp(Vertex vOther, float fLerpAmt);
		bool			IsInsideViewFrustum();
		float			Get(AXIS_COMPONENT eIndex);
	protected:
	private:
		Vector4f		m_vPos;
		Vector4f		m_vTexCoords;
		Vector4f		m_vNormal;
		Vector4f		m_vColour;
};