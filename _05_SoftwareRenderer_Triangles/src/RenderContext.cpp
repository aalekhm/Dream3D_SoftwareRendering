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

	Vertex vMinYVert(100, 50);
	Vertex vMidYVert(150, 100);
	Vertex vMaxYVert(80, 150);

	ScanConvertTriangle(vMinYVert, vMidYVert, vMaxYVert, 0);
	FillShape(50, 150);
}

void RenderContext::DrawPixel(int32_t iX, int32_t iY, Pixel pPixel)
{
	m_pFrameBuffer->SetPixel(iX, iY, pPixel);
}

void RenderContext::Clear(Pixel p)
{
	m_pFrameBuffer->Clear(p);
}

void RenderContext::DrawScanBuffer(int32_t iYCoord, int32_t iXMin, int32_t iXMax)
{
	m_iScanBuffer[iYCoord * 2 + 0] = iXMin;
	m_iScanBuffer[iYCoord * 2 + 1] = iXMax;
}

void RenderContext::FillShape(int32_t iYMin, int32_t iYMax)
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

void RenderContext::ScanConvertTriangle(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, int32_t iHandedness)
{
	ScanConvertLine(vMinYVert, vMaxYVert, 0 + iHandedness);
	ScanConvertLine(vMinYVert, vMidYVert, 1 - iHandedness);
	ScanConvertLine(vMidYVert, vMaxYVert, 1 - iHandedness);
}

void RenderContext::ScanConvertLine(Vertex vMinYVert, Vertex vMaxYVert, int32_t iWhichSide)
{
	int32_t iYStart	= vMinYVert.GetY();
	int32_t iYEnd	= vMaxYVert.GetY();
	int32_t iXStart	= vMinYVert.GetX();
	int32_t iXEnd	= vMaxYVert.GetX();

	int32_t iXDist = iXEnd - iXStart;
	int32_t iYDist = iYEnd - iYStart;
	if (iYDist < 0)
	{
		return;
	}

	float fXStep = (float)iXDist / (float)iYDist;
	float fCurX = (float)iXStart;

	for (int32_t j = iYStart; j < iYEnd; j++)
	{
		m_iScanBuffer[j * 2 + iWhichSide] = fCurX;
		fCurX += fXStep;
	}
}

RenderContext::~RenderContext()
{

}