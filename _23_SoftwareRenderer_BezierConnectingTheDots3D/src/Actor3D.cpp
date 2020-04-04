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
: m_pMesh(nullptr)
, m_pTransform(nullptr)
, m_pBitmapTexture(nullptr)
, m_bVisible(true)
, m_bConstantY(false)
{
	if (!sMeshName.empty())
	{
		m_pMesh = std::make_unique<Mesh>(sMeshName.c_str());
		m_pTransform = new Transform(	Vector4f(0, 0, 0),
										Quaternion(0, 0, 0, 1),
										Vector4f(0.1f, 0.1f, 0.1f)
										);

	}

	if (!sTexture.empty())
	{
		m_pBitmapTexture = std::make_unique<Sprite>(sTexture.c_str());
	}

	m_vPos.x = m_vPos.y = m_vPos.z = 0.0f;
	m_vScale.x = m_vScale.y = m_vScale.z = 1.0f;
	m_vHeading.x = 0.0f, m_vHeading.y = 0.0f; m_vHeading.z = 1;// Facing +Z
	m_fRotRadiansY = DEG2RAD(0);
	m_fRotRadiansX = m_fRotRadiansY = m_fRotRadiansZ = 0.0f;
}

void Actor3D::Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs)
{
	if ( !IsVisible() )
		return;

	float fDeltaMs = iDeltaTimeMs * 0.001f;

	if (m_pMesh != nullptr)
	{
		Matrix4f viewProjMat = m_pCamera->getViewProjection();
		
		if(m_bConstantY)
			m_pMesh->Draw(pRenderContext, viewProjMat, GetTransformationMatrix(), m_pBitmapTexture.get());
		else
			m_pMesh->Draw(pRenderContext, viewProjMat, m_pTransform->getTransformation(), m_pBitmapTexture.get());
	}
}

void Actor3D::SetCamera(Camera* pCamera)
{
	m_pCamera = pCamera;
}

Vector3 Actor3D::GetPosition()
{
	return m_vPos;
}

void Actor3D::SetPosition(Vector3 vPos)
{
	m_vPos = vPos;
	*m_pTransform = m_pTransform->setPos(Vector4f(m_vPos.x, m_vPos.y, m_vPos.z));
}

Vector3 Actor3D::GetScale()
{
	return m_vScale;
}

void Actor3D::SetScale(Vector3 vScale)
{
	m_vScale = vScale;
	*m_pTransform = m_pTransform->setScale(Vector4f(m_vScale.x, m_vScale.y, m_vScale.z));
}

float Actor3D::GetRotateX()
{
	return m_fRotRadiansX;
}

float Actor3D::GetRotateY()
{
	return m_fRotRadiansY;
}

float Actor3D::GetRotateZ()
{
	return m_fRotRadiansZ;
}

void Actor3D::RotateX(float fRotDegree)
{
	m_fRotRadiansX = DEG2RAD(fRotDegree);
}

void Actor3D::RotateY(float fRotDegree)
{
	m_fRotRadiansY = DEG2RAD(fRotDegree);
}

void Actor3D::RotateZ(float fRotDegree)
{
	m_fRotRadiansZ = DEG2RAD(fRotDegree);
}

Matrix4f Actor3D::GetTransformationMatrix()
{
	Matrix4f translationMatrix;
	translationMatrix.initTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	Matrix4f scaleMatrix;
	scaleMatrix.initScale(m_vScale.x, m_vScale.y, m_vScale.z);

	Matrix4f rotationMatrix;
	rotationMatrix.initRotation(/*m_fRotRadiansX*/0, m_fRotRadiansY, /*m_fRotRadiansZ*/0);

	return translationMatrix.mul(rotationMatrix.mul(scaleMatrix));
}

static bool bFlipped = false;

Matrix4f Actor3D::CalculateRotationAngles()
{
	//create a transformation matrix
	Matrix4 matTransform;

	Matrix4f translationMatrix;
	translationMatrix.initTranslation(m_vPos.x, m_vPos.y, m_vPos.z);

	Matrix4f scaleMatrix;
	scaleMatrix.initScale(m_vScale.x, m_vScale.y, m_vScale.z);

	Matrix4f rotationMatrix;
	{
		Vector3 vLookAtOrigXZ;
		vLookAtOrigXZ.x = m_vHeading.x;
		vLookAtOrigXZ.y = 0;
		vLookAtOrigXZ.z = m_vHeading.z;
		vLookAtOrigXZ.normalize();

		Vector3 vLookAtXZ;
		vLookAtXZ.x = m_vLookAt.x;
		vLookAtXZ.y = 0;
		vLookAtXZ.z = m_vLookAt.z;
		vLookAtXZ.normalize();
		{
			float fDotXZ = vLookAtOrigXZ.dot(vLookAtXZ);
			float fYAngleRad = -acosf(fDotXZ);
			float fDegree = RAD2DEG(fYAngleRad);

			if (fabs(fDotXZ - 1.0f) < EPSILON)
			{
				fYAngleRad = 0;
			}
				
			m_fRotRadiansY += fYAngleRad;
//printLine_1("vLookAtOrigYZ = %f, %f, %f ==== vLookAtYZ = %f, %f, %f", vLookAtOrigXZ.x, vLookAtOrigXZ.y, vLookAtOrigXZ.z, vLookAtXZ.x, vLookAtXZ.y, vLookAtXZ.z);
//printLine_1("m_fRotRadiansY = %f, m_fRotRadiansY = %f(%f D), fDegree = %f, fDotXZ = %f, m_vLookAt = %f, %f, %f", m_fRotRadiansY, m_fRotRadiansY, RAD2DEG(m_fRotRadiansY), fDegree, fDotXZ, m_vLookAt.x, m_vLookAt.y, m_vLookAt.z);
		}

		m_vHeading = m_vLookAt;
	}
	
	return translationMatrix.mul(rotationMatrix.mul(scaleMatrix));
}

void Actor3D::LookAtConstantY(Vector3 vLookAtPoint)
{
	m_bConstantY = true;

	m_vLookAt = (vLookAtPoint - m_vPos);
	CalculateRotationAngles();
}

void Actor3D::LookAt(Vector3 vLookAtPoint)
{
	m_vLookAt = (vLookAtPoint - m_vPos);

	Vector4f vUp = m_pTransform->getRot().getUp();
	*m_pTransform = m_pTransform->lookAt(Vector4f(vLookAtPoint.x, vLookAtPoint.y, vLookAtPoint.z), vUp);
}

bool Actor3D::IsVisible()
{
	return m_bVisible;
}

void Actor3D::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

Actor3D::~Actor3D()
{
}