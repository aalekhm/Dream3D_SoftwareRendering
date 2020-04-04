#pragma once
#include "Vector4f.h"
#include <memory>
#include <vector>
#include "Matrices.h"
#include "Matrix4f.h"

class Sprite;
class Mesh;
class Transform;
class RenderContext;
class Camera;
class Actor3D
{
	public:
									Actor3D(std::string sMeshName, std::string sTexture);
		virtual						~Actor3D();

		void						Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs);

		void						SetCamera(Camera* pCamera);

		Vector3						GetPosition();
		void						SetPosition(Vector3 vPos);

		Vector3						GetScale();
		void						SetScale(Vector3 vScale);

		Matrix4f					GetTransformationMatrix();
		void						LookAt(Vector3 vLookAtPoint);
		void						LookAtConstantY(Vector3 vLookAtPoint);
		void						lookAtToAxes(Vector3 target);
		void						lookat(Vector3& target, Vector3& upDir);
		Quaternion					get_rotation(Vector4f &a, Vector4f &b, Vector4f &up);

		bool						IsVisible();
		void						SetVisible(bool bVisible);

		float						GetRotateX();
		float						GetRotateY();
		float						GetRotateZ();

		void						RotateX(float fRotDegree);
		void						RotateY(float fRotDegree);
		void						RotateZ(float fRotDegree);
	protected:
		Matrix4f					CalculateRotationAngles();
	private:
		Vector3						m_vPos;
		Vector3						m_vScale;
		Vector3						m_vHeading;
		Vector3						m_vHeadingOrig;
		Vector3						m_vLookAt;

		Vector3						m_vRight;
		Vector3						m_vUp;

		float						m_fRotRadiansX;
		float						m_fRotRadiansY;
		float						m_fRotRadiansZ;

		Camera*						m_pCamera;
		std::unique_ptr<Mesh>		m_pMesh;
		std::unique_ptr<Sprite>		m_pBitmapTexture;
		Transform*					m_pTransform;

		bool						m_bVisible;

		bool						m_bConstantY;
};