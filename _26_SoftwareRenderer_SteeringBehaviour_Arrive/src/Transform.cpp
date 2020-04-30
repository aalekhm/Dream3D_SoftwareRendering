#include "Transform.h"

Transform::Transform()
{
	m_pos = Vector4f(0, 0, 0, 0);
	m_rot = Quaternion(0, 0, 0, 1);
	m_scale = Vector4f(1, 1, 1, 1);
}

Transform::Transform(Vector4f pos)
{
	m_pos = pos;
	m_rot = Quaternion(0, 0, 0, 1);
	m_scale = Vector4f(1, 1, 1, 1);
}

Transform::Transform(Vector4f pos, Quaternion rot, Vector4f scale)
{
	m_pos = pos;
	m_rot = rot;
	m_scale = scale;
}

Transform Transform::setPos(Vector4f pos)
{
	return Transform(pos, m_rot, m_scale);
}

Transform Transform::rotate(Quaternion rotation)
{
	return Transform(m_pos, rotation.mul(m_rot).normalized(), m_scale);
}

Transform Transform::lookAt(Vector4f point, Vector4f up)
{
	m_rot = getLookAtRotation(point, up).normalized();
	return rotate(m_rot);

	// Orig
	//return rotate(getLookAtRotation(point, up));
}

Quaternion Transform::getLookAtRotation(Vector4f point, Vector4f up)
{
	return Quaternion(	Matrix4f().initRotation(point.sub(m_pos).normalized(),up) );
}

Matrix4f Transform::getTransformation()
{
	Matrix4f translationMatrix;
	translationMatrix.initTranslation(	m_pos.getX(),
										m_pos.getY(),
										m_pos.getZ());

	Matrix4f rotationMatrix = m_rot.toRotationMatrix();

	Matrix4f scaleMatrix;
	scaleMatrix.initScale(	m_scale.getX(),
							m_scale.getY(),
							m_scale.getZ());

	return translationMatrix.mul( rotationMatrix.mul(scaleMatrix) );
}

Vector4f Transform::getTransformedPos()
{
	return m_pos;
}

Quaternion Transform::getTransformedRot()
{
	return m_rot;
}

Vector4f Transform::getPos()
{
	return m_pos;
}

Quaternion Transform::getRot()
{
	return m_rot;
}

Vector4f Transform::getScale()
{
	return m_scale;
}

Transform Transform::setScale(Vector4f scale)
{
	return Transform(m_pos, m_rot, scale);
}