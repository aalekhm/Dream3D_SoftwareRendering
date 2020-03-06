#include "RenderContext.h"
#include "Sprite.h"
#include <GL/glew.h>
#include "Stars3D.hpp"
#include "Matrix4f.h"

//Stars3D* gStars3D;

Vertex		gMinYVert( Vector4f(-1, -1, 0, 1),		Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
Vertex		gMidYVert( Vector4f(0, 1, 0, 1),		Vector4f(0.5f, 1.0f, 0.0f, 0.0f));
Vertex		gMaxYVert( Vector4f(1, -1, 0, 1),		Vector4f(1.0f, 0.0f, 0.0f, 0.0f));
float		fRotCounter = 0.0f;
Matrix4f	matProjection;
Sprite*		gTexture;

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

	gTexture = new Sprite(32, 32);
	for (int j = 0; j < gTexture->GetHeight(); j++)
	{
		for (int i = 0; i < gTexture->GetWidth(); i++)
		{
			Pixel p(rand() % 255, rand() % 255, rand() % 255);
			gTexture->SetPixel(	i,
								j,
								p);
		}
	}

	//gStars3D = new Stars3D(m_pFrameBuffer, 60.0f, 4096, 64.0f, 10.0f);
	matProjection.initPerspective(	(float)(70.0f * (3.142f / 180.0f)),
									(float)iWidth / (float)iHeight,
									0.1f,
									1000.0f);
}

uint32_t RenderContext::GetWidth()
{
	return m_iWidth;
}

uint32_t RenderContext::GetHeight()
{
	return m_iHeight;
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

	{
		fRotCounter += (iDeltaTimeMs / 1000.0f);
		Matrix4f matTranslation;
		matTranslation = matTranslation.initTranslation(0.0f, 0.0f, 3.0f);

		Matrix4f matRotation;
		matRotation = matRotation.initRotation(fRotCounter, fRotCounter, fRotCounter);
		
		Matrix4f matTransformation = matProjection.mul(matTranslation.mul(matRotation));
		
		FillTriangle(	gMaxYVert.Transform(matTransformation),
						gMidYVert.Transform(matTransformation),
						gMinYVert.Transform(matTransformation),
						gTexture);
	}
}

void RenderContext::DrawPixel(int32_t iX, int32_t iY, Pixel pPixel)
{
	m_pFrameBuffer->SetPixel(iX, iY, pPixel);
}

void RenderContext::CopyPixel(int destX, int destY, int srcX, int srcY, Sprite* pTexture)
{
	m_pFrameBuffer->SetPixel(destX, destY, pTexture->GetPixel(srcX, srcY));
}

void RenderContext::Clear(Pixel p)
{
	m_pFrameBuffer->Clear(p);
}

void RenderContext::FillTriangle(Vertex v1, Vertex v2, Vertex v3, Sprite* pTexture)
{
	Matrix4f screenSpaceTransform;
	screenSpaceTransform.initScreenSpaceTransform(GetWidth() / 2, GetHeight() / 2);

	Vertex vMinYVert = v1.Transform(screenSpaceTransform).PerspectiveDivide();
	Vertex vMidYVert = v2.Transform(screenSpaceTransform).PerspectiveDivide();
	Vertex vMaxYVert = v3.Transform(screenSpaceTransform).PerspectiveDivide();

	if (vMaxYVert.GetY() < vMidYVert.GetY())
	{
		Vertex vTemp = vMidYVert;
		vMidYVert = vMaxYVert;
		vMaxYVert = vTemp;
	}

	if (vMidYVert.GetY() < vMinYVert.GetY())
	{
		Vertex vTemp = vMinYVert;
		vMinYVert = vMidYVert;
		vMidYVert = vTemp;
	}

	if (vMaxYVert.GetY() < vMidYVert.GetY())
	{
		Vertex vTemp = vMidYVert;
		vMidYVert = vMaxYVert;
		vMaxYVert = vTemp;
	}

	float fArea = vMinYVert.TriangleAreaTimesTwo(vMaxYVert, vMidYVert);
	bool bHandedness = (fArea >= 0);

	ScanTriangle(vMinYVert, vMidYVert, vMaxYVert, bHandedness, pTexture);
}

void RenderContext::ScanTriangle(Vertex vMinYVert, Vertex vMidYVert, Vertex vMaxYVert, bool bHandedness, Sprite* pTexture)
{
	Gradients gradients(vMinYVert, vMidYVert, vMaxYVert);
	Edge topToBottom(gradients, vMinYVert, vMaxYVert, 0);
	Edge topToMiddle(gradients, vMinYVert, vMidYVert, 0);
	Edge middleToBottom(gradients, vMidYVert, vMaxYVert, 1);

	ScanEdges(&gradients, &topToBottom, &topToMiddle, bHandedness, pTexture);
	ScanEdges(&gradients, &topToBottom, &middleToBottom, bHandedness, pTexture);
}

void RenderContext::ScanEdges(Gradients* pGradients, Edge* eA, Edge* eB, bool handedness, Sprite* pTexture)
{
	Edge* eLeft = eA;
	Edge* eRight = eB;
	if (handedness)
	{
		Edge* eTemp = eLeft;
		eLeft = eRight;
		eRight = eTemp;
	}

	int yStart = eB->GetYStart();
	int yEnd = eB->GetYEnd();
	for (int j = yStart; j < yEnd; j++)
	{
		DrawScanLine(pGradients, eLeft, eRight, j, pTexture);

		eLeft->Step();
		eRight->Step();
	}
}

void RenderContext::DrawScanLine(Gradients* pGradients, Edge* eLeft, Edge* eRight, int j, Sprite* pTexture)
{
	int32_t xMin = (int32_t)ceil(eLeft->GetX());
	int32_t xMax = (int32_t)ceil(eRight->GetX());

	float fXPreStep = xMin - eLeft->GetX();

	// Re-Calculate 'Step' due to floating point precisions.
	float fXDist = eRight->GetX() - eLeft->GetX();

	float fTexCoordXXStep = (eRight->GetTexCoordX() - eLeft->GetTexCoordX()) / fXDist;
	float fTexCoordYXStep = (eRight->GetTexCoordY() - eLeft->GetTexCoordY()) / fXDist;
	float fOneOverZXStep = (eRight->GetOneOverZ() - eLeft->GetOneOverZ()) / fXDist;

	float fTexCoordX = eLeft->GetTexCoordX() + fTexCoordXXStep * fXPreStep;
	float fTexCoordY = eLeft->GetTexCoordY() + fTexCoordYXStep * fXPreStep;
	float fOneOverZ = eLeft->GetOneOverZ() + fOneOverZXStep * fXPreStep;

	for (int i = xMin; i < xMax; i++)
	{
		float z = 1.0f / fOneOverZ;
		int srcX = (int)((fTexCoordX * z) * (float)(pTexture->GetWidth() - 1) + 0.5f);
		int srcY = (int)((fTexCoordY * z) * (float)(pTexture->GetHeight() - 1) + 0.5f);

		CopyPixel(i, j, srcX, srcY, pTexture);

		fOneOverZ += pGradients->GetOneOverZXStep();
		fTexCoordX += pGradients->GetTexCoordXXStep();
		fTexCoordY += pGradients->GetTexCoordYXStep();
	}
}

RenderContext::~RenderContext()
{

}