#pragma once

#include "Engine/EngineManager.h"
#include "Sprite.h"
#include "Pixel.h"
#include <functional>
#include "Vector4f.h"

class RenderContext;
class Mesh;
class Dream3DTest : EngineManager
{
	public:
										Dream3DTest();
		virtual							~Dream3DTest();

		virtual void					initialize();
		virtual void					update(float elapsedTime);
		virtual void					render(float elapsedTime);
		void							onRender(uint32_t iDeltaTimeMs);

		virtual void					keyPressedEx(unsigned int iVirtualKeycode, unsigned short ch);
		virtual void					keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch);

		virtual void					onMouseDownEx(int mCode, int x, int y);
		virtual void					onMouseMoveEx(int mCode, int x, int y);
		virtual void					onMouseUpEx(int mCode, int x, int y);
		virtual void					onMouseWheelEx(WPARAM wParam, LPARAM lParam);
	protected:
		void							initWorld();
		void							init3D();
		void							init2D();

		void							render3D(uint32_t iDeltaTimeMs);
		void							render2D(uint32_t iDeltaTimeMs);

		Vector4f						GetBeizerPoint(std::vector<Vector4f>& vControlPoints, float fTime);
	private:
		RenderContext*					m_pGraphics;

		std::function<void(uint32_t)>	m_fRenderCallback;
};
