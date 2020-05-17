#pragma once
#include "Vector4f.h"
#include <vector>
#include "Transform.h"
#include "Matrices.h"

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
	void								LookAt(Vector4f vPoint);
	Sprite*								GetTexture();
	std::vector<Shape2DVertex>&			GetVertices();
	Transform*							GetTransform();
	void								Rotate(float fRotZ);

	void								Update(RenderContext* pRenderContext, float fDeltaMs, Matrix3 matTransformation);
	void								Render(RenderContext* pRenderContext, Matrix3 matTransformation);

	Vector4f							TransformPoint(Vector4f vPoint, Matrix3 matTransformation);

	std::vector<Shape2DVertex>			m_vVertices;
	Sprite*								m_pBitmapTexture;
	Transform							m_pTransform;
	Matrix3								m_TransformMat;
};