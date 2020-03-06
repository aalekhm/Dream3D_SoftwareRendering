#include "Dream3DTest.h"
#include "RenderContext.h"

Dream3DTest		engine;

Dream3DTest::Dream3DTest()
{
	setViewport(256, 256);
	setPixelSize(1, 1);
}

Dream3DTest::~Dream3DTest()
{
}

void Dream3DTest::initialize()
{
	m_pRenderContext = new RenderContext(EngineManager::getWidth(), EngineManager::getHeight(), EngineManager::getPixelWidth(), EngineManager::getPixelHeight());
}

void Dream3DTest::update(float elapsedTime)
{

}

void Dream3DTest::render(float elapsedTime)
{
	m_pRenderContext->Render();
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