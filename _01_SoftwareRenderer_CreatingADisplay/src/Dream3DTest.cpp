#include "Dream3DTest.h"
#include "Common/CCString.h"

Dream3DTest engine;

void printLine(const char* lpszFormat, ...)
{
	va_list args;
	va_start(args, lpszFormat);
	{
		CCString cBuff;
		char sBuffer[512];
		_vsnprintf(sBuffer, 511, lpszFormat, args);

		cBuff = sBuffer;
		cBuff += "\n";

		OutputDebugString(cBuff.c_str());
	}
	va_end(args);
}

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
	m_pDefaultDrawTarget = new Sprite(EngineManager::getWidth(), EngineManager::getHeight());

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &glBuffer);
	glBindTexture(GL_TEXTURE_2D, glBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, EngineManager::getWidth(), EngineManager::getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pDefaultDrawTarget->GetData());
}

void Dream3DTest::update(float elapsedTime)
{

}

void Dream3DTest::render(float elapsedTime)
{
	glViewport(0, 0, EngineManager::getWidth() * EngineManager::getPixelWidth(), EngineManager::getHeight() * EngineManager::getPixelHeight());

	onRender(elapsedTime);
	
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, EngineManager::getWidth(), EngineManager::getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, m_pDefaultDrawTarget->GetData());
	
	float fSubPixelOffsetX = 0.0f, fSubPixelOffsetY = 0.0f;
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f);
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f + (fSubPixelOffsetX), 1.0f + (fSubPixelOffsetY), 0.0f);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f + (fSubPixelOffsetX), 1.0f + (fSubPixelOffsetY), 0.0f);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f);
	glEnd();
}

void Dream3DTest::onRender(float elapsedTime)
{
	Clear(Pixel(255, 0, 0, 255));

	for (int x = 0; x < EngineManager::getWidth(); x++)
	{
		for (int y = 0; y < EngineManager::getHeight(); y++)
		{
			draw(x, y, Pixel(rand() % 255, rand() % 255, rand() % 255));
		}
	}

	printLine("FPS = %d", EngineManager::getFPS());
}
	 


void Dream3DTest::draw(int32_t iX, int32_t iY, Pixel pPixel)
{
	m_pDefaultDrawTarget->SetPixel(iX, iY, pPixel);
}

void Dream3DTest::Clear(Pixel p)
{
	m_pDefaultDrawTarget->Clear(p);
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