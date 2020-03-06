# pragma once
#include "Matrix4f.h"
#include "Transform.h"
#include "Vector4f.h"

class Camera
{
public:
						Camera(Matrix4f projection);
	Matrix4f			getViewProjection();
	void				update(float delta);
	void				move(Vector4f dir, float amt);
	void				rotate(Vector4f axis, float angle);
protected:
private:
	static Vector4f		Y_AXIS;
	Transform 			m_transform;
	Matrix4f 			m_projection;
	Transform			getTransform();
};