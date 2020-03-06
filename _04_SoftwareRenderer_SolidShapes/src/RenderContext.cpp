#include "RenderContext.h"
#include "Sprite.h"
#include <GL/glew.h>
#include "Stars3D.hpp"

Stars3D* gStars3D;

#define _FRAMEBUFFER_PRE_RENDER_ \
	glViewport(0, 0, m_iWidth * m_iPixelWidth, m_iHeight * m_iPixelHeight); \
	auto now = std::chrono::high_resolution_clock::now(); \
	m_iDeltaTimeMs = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_tpLastUpdate).count(); \
	m_tpLastUpdate = now; \

#define _FRAMEBUFFER_POST_RENDER_ \
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_iWidth, m_iHeight, GL_RGBA, GL_UNSIGNED_BYTE, m_pFrameBuffer->GetData()); \
	\
	float fSubPixelOffsetX = 0.0f, fSubPixelOffsetY = 0.0f; \
	glBegin(GL_QUADS); \
		glTexCoord2f(0.0, 1.0); glVertex3f(-1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f); \
		glTexCoord2f(0.0, 0.0); glVertex3f(-1.0f + (fSubPixelOffsetX), 1.0f + (fSubPixelOffsetY), 0.0f); \
		glTexCoord2f(1.0, 0.0); glVertex3f(1.0f + (fSubPixelOffsetX), 1.0f + (fSubPixelOffsetY), 0.0f); \
		glTexCoord2f(1.0, 1.0); glVertex3f(1.0f + (fSubPixelOffsetX), -1.0f + (fSubPixelOffsetY), 0.0f); \
	glEnd(); \

RenderContext::RenderContext(uint32_t iWidth, uint32_t iHeight, uint32_t iPixelWidth, uint32_t iPixelHeight)
: m_iWidth(iWidth)
, m_iHeight(iHeight)
, m_iPixelWidth(iPixelWidth)
, m_iPixelHeight(iPixelHeight)
{
	m_pFrameBuffer = new Sprite(iWidth, iHeight);
	{
		GLuint glBuffer;

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &glBuffer);
		glBindTexture(GL_TEXTURE_2D, glBuffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_pFrameBuffer->GetData());
	}

	m_iDeltaTimeMs = 0;
	m_tpLastUpdate = std::chrono::high_resolution_clock::now();

	m_iScanBuffer = new uint32_t[iHeight * 2];

	gStars3D = new Stars3D(m_pFrameBuffer, 60.0f, 4096, 64.0f, 10.0f);
}

void RenderContext::Render()
{
	_FRAMEBUFFER_PRE_RENDER_
	{
		RenderInternal(m_iDeltaTimeMs);
	}
	_FRAMEBUFFER_POST_RENDER_
}

void RenderContext::RenderInternal(uint32_t iDeltaTimeMs)
{
	Clear(Pixel(0, 0, 0, 0xFF));

	for (int32_t i = 50; i < 100; i++)
	{
		DrawScanBuffer(i, 128 - i, 128 + i);
	}
	FillShape(50, 100);
}

void RenderContext::DrawPixel(int32_t iX, int32_t iY, Pixel pPixel)
{
	m_pFrameBuffer->SetPixel(iX, iY, pPixel);
}

void RenderContext::Clear(Pixel p)
{
	m_pFrameBuffer->Clear(p);
}

void RenderContext::DrawScanBuffer(uint32_t iYCoord, uint32_t iXMin, uint32_t iXMax)
{
	m_iScanBuffer[iYCoord * 2 + 0] = iXMin;
	m_iScanBuffer[iYCoord * 2 + 1] = iXMax;
}

void RenderContext::FillShape(uint32_t iYMin, uint32_t iYMax)
{
	Pixel p(0xFF, 0xFF, 0xFF, 0xFF);
	for (int32_t y = iYMin; y < iYMax; y++)
	{
		uint32_t iXMin = m_iScanBuffer[y * 2 + 0];
		uint32_t iXMax = m_iScanBuffer[y * 2 + 1];

		for (int32_t x = iXMin; x < iXMax; x++)
		{
			DrawPixel(x, y, p);
		}
	}
}

RenderContext::~RenderContext()
{

}