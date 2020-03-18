#include "Camera.h"
#include "Quaternion.h"
#include "Transform.h"
#include "Vector4f.h"
#include "Engine/EngineManager.h"

Vector4f Camera::Y_AXIS(0.0f, 1.0f, 0.0f, 0.0f);

Camera::Camera(Matrix4f projection)
{
	m_projection = projection;
}

Matrix4f Camera::getViewProjection()
{
	Matrix4f cameraRotation = getTransform().getTransformedRot().conjugate().toRotationMatrix();
	Vector4f cameraPos = getTransform().getTransformedPos().mul(-1);

	Matrix4f cameraTranslation;
	cameraTranslation.initTranslation(	cameraPos.getX(),
										cameraPos.getY(),
										cameraPos.getZ());

	return m_projection.mul(cameraRotation.mul(cameraTranslation));
}

void Camera::update(float delta)
{
	// Speed and rotation amounts are hardcoded here.
	// In a more general system, you might want to have them as variables.
	static float sensitivityX = 0.5f * delta;
	static float sensitivityY = 0.5f * delta;
	static float movAmt = 1.0f * delta;

	// Similarly, input keys are hardcoded here.
	// As before, in a more general system, you might want to have these as variables.
	if (EngineManager::getInstance()->isKeyPressed('W') || EngineManager::getInstance()->isKeyPressed('w'))
		move(getTransform().getRot().getForward(), movAmt);

	if (EngineManager::getInstance()->isKeyPressed('S') || EngineManager::getInstance()->isKeyPressed('s'))
		move(getTransform().getRot().getForward(), -movAmt);

	if (EngineManager::getInstance()->isKeyPressed('A') || EngineManager::getInstance()->isKeyPressed('a'))
		move(getTransform().getRot().getLeft(), movAmt);

	if (EngineManager::getInstance()->isKeyPressed('D') || EngineManager::getInstance()->isKeyPressed('d'))
		move(getTransform().getRot().getRight(), movAmt);

	if (EngineManager::getInstance()->isKeyPressed('E') || EngineManager::getInstance()->isKeyPressed('e'))
		rotate(Y_AXIS, sensitivityX);

	if (EngineManager::getInstance()->isKeyPressed('Q') || EngineManager::getInstance()->isKeyPressed('q'))
		rotate(Y_AXIS, -sensitivityX);

	if (EngineManager::getInstance()->isKeyPressed('R') || EngineManager::getInstance()->isKeyPressed('r'))
		rotate(getTransform().getRot().getRight(), -sensitivityY);

	if (EngineManager::getInstance()->isKeyPressed('F') || EngineManager::getInstance()->isKeyPressed('f'))
		rotate(getTransform().getRot().getRight(), sensitivityY);
}

void Camera::move(Vector4f dir, float amt)
{
	m_transform = getTransform().setPos(getTransform().getPos().add(dir.mul(amt)));
}

void Camera::rotate(Vector4f axis, float angle)
{
	m_transform = getTransform().rotate(Quaternion(axis, angle));
}

Transform Camera::getTransform()
{
	return m_transform;
}
