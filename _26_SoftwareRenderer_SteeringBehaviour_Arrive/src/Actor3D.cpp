#include "Actor3D.h"
#include "RenderContext.h"
#include "Sprite.h"
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Common/CCString.h"

#define RAD2DEG(__angleInRadians__) (180 * __angleInRadians__ / 3.141593f)
#define DEG2RAD(__angleInDegrees__) (3.141593f * __angleInDegrees__ / 180)
const float EPSILON = 0.00001f;

void printLine_1(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	{
		CCString cBuff;
		char sBuffer[512];
		_vsnprintf(sBuffer, 511, lpszFormat, args);

		cBuff = sBuffer;
		cBuff += "\n";

		OutputDebugString(cBuff.c_str());
	}
	va_end(args);
}

Actor3D::Actor3D(std::string sMeshName, std::string sTexture)
: Actor(sTexture)
, m_pMesh(nullptr)
, m_pCamera(nullptr)
, m_pTransform(nullptr)
{
	m_bIs2D = false;
	if (!sMeshName.empty())
	{
		m_pMesh = std::make_unique<Mesh>(sMeshName.c_str());
		m_pTransform = new Transform(	Vector4f(0, 0, 0),
										Quaternion(0, 0, 0, 1),
										Vector4f(0.1f, 0.1f, 0.1f)
										);

	}
}

void Actor3D::Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs)
{
	if ( !IsVisible() )
		return;

	Actor::Update(pRenderContext, iDeltaTimeMs);

	if (m_pMesh != nullptr && m_pCamera != nullptr)
	{
		Matrix4f viewProjMat = m_pCamera->getViewProjection();
		m_pMesh->Draw(pRenderContext, viewProjMat, m_pTransform->getTransformation(), GetTexture());
	}
}

void Actor3D::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

void Actor3D::OnSetPosition()
{
	*m_pTransform = m_pTransform->setPos(Vector4f(m_vPos.x, m_vPos.y, m_vPos.z));
}

void Actor3D::OnSetScale()
{
	*m_pTransform = m_pTransform->setScale(Vector4f(m_vScale.x, m_vScale.y, m_vScale.z));
}

void Actor3D::OnLookAt(Vector3 vLookAtPoint)
{
	Vector4f vUp = m_pTransform->getRot().getUp();
	*m_pTransform = m_pTransform->lookAt(Vector4f(vLookAtPoint.x, vLookAtPoint.y, vLookAtPoint.z), vUp);
}

Actor3D::~Actor3D()
{
	delete m_pTransform;
}