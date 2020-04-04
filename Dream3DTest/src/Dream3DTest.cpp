#include "Dream3DTest.h"
#include "Engine/Console2D.h"

Dream3DTest engine;

Dream3DTest::Dream3DTest()
{
}

Dream3DTest::~Dream3DTest()
{
}

void Dream3DTest::initialize()
{

}

void Dream3DTest::update(float elapsedTime)
{

}

void Dream3DTest::render(float elapsedTime)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClearColor(1, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT + GL_DEPTH_BUFFER_BIT);

	render2DConsole();
}

void Dream3DTest::render2DConsole()
{
	Console2D* pConsole2D = Console2D::getInstance();
	pConsole2D->updateConsole();
	{
		pConsole2D->setClip(0, 0, getInstance()->getWidth(), getInstance()->getHeight());
		pConsole2D->fillRect(COLOR_WHITE, new Rect(0, 0, 10, 25));
		pConsole2D->drawStringFont("Test String", 10, 10, 0);
	}
	pConsole2D->renderConsole();
}
	 
void Dream3DTest::keyPressedEx(unsigned int iVirtualKeycode, unsigned short ch)
{

}

void Dream3DTest::keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch)
{

}
	 
void Dream3DTest::onMouseDownEx(int mCode, int x, int y)
{

}

void Dream3DTest::onMouseMoveEx(int mCode, int x, int y)
{

}

void Dream3DTest::onMouseUpEx(int mCode, int x, int y)
{

}

void Dream3DTest::onMouseWheelEx(WPARAM wParam, LPARAM lParam)
{

}