#include "Dream3DTest.h"
#include "Common/CCString.h"
#include "RenderContext.h"

Dream3DTest		engine;

//void printLine_(const char* lpszFormat, ...)
//{
//	va_list args;
//	va_start(args, lpszFormat);
//	{
//		CCString cBuff;
//		char sBuffer[512];
//		_vsnprintf(sBuffer, 511, lpszFormat, args);
//
//		cBuff = sBuffer;
//		cBuff += "\n";
//
//		OutputDebugString(cBuff.c_str());
//	}
//	va_end(args);
//}

Dream3DTest::Dream3DTest()
{
	setViewport(480, 320);
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