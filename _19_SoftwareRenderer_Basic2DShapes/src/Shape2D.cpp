#include "Shape2D.h"
#include "Sprite.h"
#include "RenderContext.h"

Shape2DVertex::Shape2DVertex(Vector4f vPos, Vector4f vTexCoords, Vector4f vColour)
	: m_vPos(vPos)
	, m_vTexCoords(vTexCoords)
	, m_vColour(vColour)
{
}

Shape2D::Shape2D(std::vector<Shape2DVertex>& vVertices, Sprite* pBitmapTexture)
	: m_pBitmapTexture(pBitmapTexture)
{
	m_vVertices = vVertices;
}

void Shape2D::SetPosition(Vector4f vPos)
{
	m_pTransform = m_pTransform.setPos(vPos);
}

Vector4f Shape2D::GetPosition()
{
	return m_pTransform.getPos();
}

Sprite* Shape2D::GetTexture()
{
	return m_pBitmapTexture;
}

std::vector<Shape2DVertex>& Shape2D::GetVertices()
{
	return m_vVertices;
}

Transform* Shape2D::GetTransform()
{
	return &m_pTransform;
}

void Shape2D::Rotate(float fRotZ)
{
	m_pTransform = m_pTransform.rotate(Quaternion(Vector4f(0, 0, 1), fRotZ));
}

void Shape2D::Update(RenderContext* pRenderContext, float fDeltaMs)
{
	Rotate(fDeltaMs);
	
	Render(pRenderContext);
}

void Shape2D::Render(RenderContext* pRenderContext)
{
	m_pBitmapTexture->SetTextureAlpha(0.5f);
	{
		for (int32_t i = 1; (i + 1) < m_vVertices.size(); i++)
		{
			Shape2DVertex v2DV1 = m_vVertices[0];
			Shape2DVertex v2DV2 = m_vVertices[i];
			Shape2DVertex v2DV3 = m_vVertices[i + 1];

			Vertex v1 = Vertex(	v2DV1.m_vPos,
								v2DV1.m_vTexCoords, 
								Vector4f(0, 0, -1),
								v2DV1.m_vColour);
			v1 = v1.Transform(m_pTransform.getTransformation());

			Vertex v2 = Vertex( v2DV2.m_vPos,
								v2DV2.m_vTexCoords, 
								Vector4f(0, 0, -1),
								v2DV2.m_vColour);
			v2 = v2.Transform(m_pTransform.getTransformation());

			Vertex v3 = Vertex( v2DV3.m_vPos,
								v2DV3.m_vTexCoords, 
								Vector4f(0, 0, -1),
								v2DV3.m_vColour);
			v3 = v3.Transform(m_pTransform.getTransformation());

			pRenderContext->FillTriangle2D(	v1,
											v2,
											v3,
											m_pBitmapTexture);
		}
	}
	m_pBitmapTexture->SetTextureAlpha(1.0f);
}