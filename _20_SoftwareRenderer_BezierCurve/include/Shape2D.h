#pragma once
#include "Vector4f.h"
#include <vector>
#include "Transform.h"

class Sprite;
class RenderContext;
struct Shape2DVertex
{
						Shape2DVertex(Vector4f vPos, Vector4f vTexCoords, Vector4f vColour);
	Vector4f			m_vPos;
	Vector4f			m_vTexCoords;
	Vector4f			m_vColour;
};

struct Shape2D
{
										Shape2D(std::vector<Shape2DVertex>& vVertices, Sprite* pBitmapTeture);

	void								SetPosition(Vector4f vPos);
	Vector4f							GetPosition();
	Sprite*								GetTexture();
	std::vector<Shape2DVertex>&			GetVertices();
	Transform*							GetTransform();
	void								Rotate(float fRotZ);

	void								Update(RenderContext* pRenderContext, float fDeltaMs);
	void								Render(RenderContext* pRenderContext);

	std::vector<Shape2DVertex>			m_vVertices;
	Sprite*								m_pBitmapTexture;
	Transform							m_pTransform;
};