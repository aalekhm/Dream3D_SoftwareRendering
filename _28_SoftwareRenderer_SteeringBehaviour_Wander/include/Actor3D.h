#pragma once
#include "Actor.h"
#include "Vector4f.h"
#include <memory>

class Mesh;
class Transform;
class RenderContext;
class Camera;
class Actor3D : public Actor
{
	public:
									Actor3D(std::string sMeshName, std::string sTexture);
		virtual						~Actor3D();

		virtual void				Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs);

		void						SetCamera(Camera* pCamera);
		virtual Vector3				GetForward();
	protected:
		virtual void				OnSetPosition();
		virtual void				OnSetScale();

		virtual void				OnLookAt(Vector3 vLookAtPoint);
	private:
		Camera*						m_pCamera;
		std::unique_ptr<Mesh>		m_pMesh;
		Transform*					m_pTransform;
};