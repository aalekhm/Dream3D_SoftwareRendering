#include "Actor2D.h"
#include "Vector4f.h"

Actor2D::Actor2D(std::vector<Shape2DVertex>& vShape, std::string sSpriteName)
: Actor(sSpriteName)
, m_pShape2D(nullptr)
{
	m_bIs2D = true;
	m_pShape2D = std::make_unique<Shape2D>(vShape, GetTexture());
}

void Actor2D::Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs)
{
	if ( !IsVisible() )
		return;

	Actor::Update(pRenderContext, iDeltaTimeMs);

	float fDeltaMs = iDeltaTimeMs * 0.001f;
	if (m_pShape2D != nullptr)
	{
		m_pShape2D->Update(pRenderContext, fDeltaMs, GetTransformationMatrix());
	}
}

Vector3 Actor2D::GetForward()
{
	return m_vHeading;
}

Matrix3 Actor2D::GetTransformationMatrix()
{
	//create a transformation matrix
	Matrix3 matTransform;

	// Scale
	matTransform.scale(m_vScale.x, m_vScale.y);

	// Rotate
	const float EPSILON = 0.00000001f;
	Vector2 vHeading(m_vHeading.x, m_vHeading.y);
	if (vHeading.lengthSq() < EPSILON)
	{
		vHeading = Vector2(-1, 0);
	}
	Vector2 vRight = vHeading.perpendicular();
	matTransform.rotate(vHeading.normalize(), vRight.normalize());

	// Translate
	matTransform.translate(m_vPos.x, m_vPos.y);

	return matTransform;
}

Actor2D::~Actor2D()
{
}