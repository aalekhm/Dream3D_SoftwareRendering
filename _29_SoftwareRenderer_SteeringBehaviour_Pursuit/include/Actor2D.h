#pragma once
#include "Actor.h"
#include "Shape2D.h"
#include <memory>

class Actor2D : public Actor
{
	public:
																		Actor2D(std::vector<Shape2DVertex>& vShape, std::string sSpriteName);
		virtual															~Actor2D();

		virtual void													Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs);
		virtual Vector3													GetForward();
	protected:
	private:
		Matrix3															GetTransformationMatrix();

		std::unique_ptr<Shape2D>										m_pShape2D;
};