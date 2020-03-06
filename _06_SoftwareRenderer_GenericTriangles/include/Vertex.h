#pragma once

class Vertex
{
	public:
						Vertex(float fX, float fY);

		float			GetX();
		float			GetY();

		void			SetX(float fX);
		void			SetY(float fY);

		float			TriangleArea(Vertex b, Vertex c);
	protected:
	private:
		float			m_fX;
		float			m_fY;
};