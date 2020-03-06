#pragma once

#include "Engine/EngineManager.h"
#include "Sprite.h"
#include "Pixel.h"

class Dream3DTest : EngineManager
{
	public:
								Dream3DTest();
		virtual					~Dream3DTest();

		virtual void			initialize();
		virtual void			update(float elapsedTime);
		virtual void			render(float elapsedTime);

		virtual void			keyPressedEx(unsigned int iVirtualKeycode, unsigned short ch);
		virtual void			keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch);

		virtual void			onMouseDownEx(int mCode, int x, int y);
		virtual void			onMouseMoveEx(int mCode, int x, int y);
		virtual void			onMouseUpEx(int mCode, int x, int y);
		virtual void			onMouseWheelEx(WPARAM wParam, LPARAM lParam);

		void					onRender(float elapsedTime);

		void					draw(int32_t iX, int32_t iY, Pixel pPixel);

		void					Clear(Pixel p);
	protected:
	private:
		GLuint					glBuffer;
		Sprite*					m_pDefaultDrawTarget = nullptr;
};
