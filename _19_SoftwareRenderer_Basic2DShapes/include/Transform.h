#pragma once
#include "Vector4f.h"
#include "Matrix4f.h"
#include "Quaternion.h"

class Transform
{
	public:
							Transform();
							Transform(Vector4f pos);
							Transform(Vector4f pos, Quaternion rot, Vector4f scale);
							Transform setPos(Vector4f pos);
							Transform rotate(Quaternion rotation);
							Transform lookAt(Vector4f point, Vector4f up);
		Quaternion			getLookAtRotation(Vector4f point, Vector4f up);
		Matrix4f			getTransformation();
		Vector4f			getTransformedPos();
		Quaternion			getTransformedRot();
		Vector4f			getPos();
		Quaternion			getRot();
		Vector4f			getScale();
	protected:
	private:
		Vector4f			m_pos;
		Quaternion			m_rot;
		Vector4f			m_scale;
};
