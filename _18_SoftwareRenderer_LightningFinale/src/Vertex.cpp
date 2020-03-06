#include "Vertex.h"

Vertex::Vertex()
{
	m_vPos = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	m_vTexCoords = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	m_vNormal = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
}

Vertex::Vertex(Vector4f vPos, Vector4f vTexCoords, Vector4f vNormal)
{
	m_vPos = vPos;
	m_vTexCoords = vTexCoords;
	m_vNormal = vNormal;
}

Vertex::Vertex(Vector4f vPos)
{
	m_vPos = vPos;
	m_vTexCoords = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
	m_vNormal = Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
}

float Vertex::GetX()
{
	return m_vPos.getX();
}

float Vertex::GetY()
{
	return m_vPos.getY();
}

float Vertex::GetZ()
{
	return m_vPos.getZ();
}

Vector4f Vertex::GetPosition()
{
	return m_vPos;
}

Vector4f Vertex::GetTexCoords()
{
	return m_vTexCoords;
}

Vector4f Vertex::GetNormal() 
{ 
	return m_vNormal; 
}

void Vertex::SetX(float fX)
{
	m_vPos.x = fX;
}

void Vertex::SetY(float fY)
{
	m_vPos.y = fY;
}

void Vertex::SetZ(float fZ)
{
	m_vPos.z = fZ;
}

float Vertex::TriangleAreaTimesTwo(Vertex b, Vertex c)
{
	float x1 = b.GetX() - m_vPos.x;
	float y1 = b.GetY() - m_vPos.y;

	float x2 = c.GetX() - m_vPos.x;
	float y2 = c.GetY() - m_vPos.y;

	return (x1 * y2 - x2 * y1);
}

Vertex Vertex::Transform(Matrix4f transform, Matrix4f matNormalTransform)
{
	return Vertex(	transform.transform(m_vPos), m_vTexCoords, matNormalTransform.transform(m_vNormal).normalized() );
}

Vertex Vertex::PerspectiveDivide()
{
	return Vertex(Vector4f(	m_vPos.getX() / m_vPos.getW(),
							m_vPos.getY() / m_vPos.getW(),
							m_vPos.getZ() / m_vPos.getW(),
							m_vPos.getW()), 
							m_vTexCoords,
							m_vNormal);
}

std::string Vertex::ToString()
{
	return (m_vPos.toString() + " " + m_vTexCoords.toString());
}

Vertex Vertex::Lerp(Vertex vOther, float fLerpAmt)
{
	return Vertex(	m_vPos.lerp(vOther.GetPosition(), fLerpAmt),
					m_vTexCoords.lerp(vOther.GetTexCoords(), fLerpAmt),
					m_vNormal.lerp(vOther.GetNormal(), fLerpAmt));
}

bool Vertex::IsInsideViewFrustum()
{
	return 	abs(m_vPos.getX()) <= abs(m_vPos.getW())
			&&
			abs(m_vPos.getY()) <= abs(m_vPos.getW())
			&&
			abs(m_vPos.getZ()) <= abs(m_vPos.getW());
}

float Vertex::Get(AXIS_COMPONENT eIndex)
{
	switch(eIndex)
	{
		case AXIS_COMPONENT::AXIS_X:
			return m_vPos.getX();
		case AXIS_COMPONENT::AXIS_Y:
			return m_vPos.getY();
		case AXIS_COMPONENT::AXIS_Z:
			return m_vPos.getZ();
		case AXIS_COMPONENT::AXIS_W:
			return m_vPos.getW();
	}
}