#include "Actor2D.h"
#include "Sprite.h"

Actor2D::Actor2D(std::vector<Shape2DVertex>& vShape, std::string sSpriteName)
: m_pShape2D(nullptr)
, m_pBitmapTexture(nullptr)
, m_bVisible(true)
{
	if (!sSpriteName.empty())
	{
		m_pBitmapTexture = std::make_unique<Sprite>(sSpriteName.c_str());
	}

	m_pShape2D = std::make_unique<Shape2D>(vShape, m_pBitmapTexture.get());

	m_vPos.x = m_vPos.y = 0.0f;
	m_vScale.x = m_vScale.y = 1.0f;
	m_vHeading.x = 1.0f, m_vHeading.y = 0.0f; // Facing Right(X-Axis)
}

void Actor2D::Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs)
{
	if ( !IsVisible() )
		return;

	float fDeltaMs = iDeltaTimeMs * 0.001f;

	if (m_pShape2D != nullptr)
	{
		m_pShape2D->Update(pRenderContext, fDeltaMs, GetTransformationMatrix());
	}
}

Vector2 Actor2D::GetPosition()
{
	return m_vPos;
}

void Actor2D::SetPosition(Vector2 vPos)
{
	m_vPos = vPos;
}

Vector2 Actor2D::GetScale()
{
	return m_vScale;
}

void Actor2D::SetScale(Vector2 vScale)
{
	m_vScale = vScale;
}

Matrix3 Actor2D::GetTransformationMatrix()
{
	//create a transformation matrix
	Matrix3 matTransform;

	//scale
	matTransform.scale(m_vScale.x, m_vScale.y);

	//rotate
	Vector2 vRight = m_vHeading.perpendicular();
	matTransform.rotate(m_vHeading.normalize(), vRight.normalize());

	//and translate
	matTransform.translate(m_vPos.x, m_vPos.y);

	return matTransform;
}

void Actor2D::LookAt(Vector2 vLookAtPoint)
{
	m_vHeading = (m_vPos - vLookAtPoint);
}

bool Actor2D::IsVisible()
{
	return m_bVisible;
}

void Actor2D::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

Actor2D::~Actor2D()
{
}