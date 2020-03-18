#pragma once
#include "Vector4f.h"
#include <memory>
#include <vector>
#include "Shape2D.h"

class Sprite;
class Actor2D
{
	public:
									Actor2D(std::vector<Shape2DVertex>& vShape, std::string sSpriteName);
		virtual						~Actor2D();

		void						Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs);

		Vector2						GetPosition();
		void						SetPosition(Vector2 vPos);

		Vector2						GetScale();
		void						SetScale(Vector2 vPos);

		Matrix3						GetTransformationMatrix();
		void						LookAt(Vector2 vLookAtPoint);

		bool						IsVisible();
		void						SetVisible(bool bVisible);
	protected:
	private:
		Vector2						m_vPos;
		Vector2						m_vScale;
		Vector2						m_vHeading;

		std::unique_ptr<Shape2D>	m_pShape2D;
		std::unique_ptr<Sprite>		m_pBitmapTexture;

		bool						m_bVisible;
};