#include "RenderContext.h"
#include "Sprite.h"
#include <GL/glew.h>
//#include "Stars3D.hpp"
#include "Matrix4f.h"

//Stars3D* gStars3D;

Vertex		gMinYVert( Vector4f(-1, -1, 0, 1),		Vector4f(0.0f, 0.0f, 0.0f, 0.0f));
Vertex		gMidYVert( Vector4f(0, 1, 0, 1),		Vector4f(0.5f, 1.0f, 0.0f, 0.0f));
Vertex		gMaxYVert( Vector4f(1, -1, 0, 1),		Vector4f(1.0f, 0.0f, 0.0f, 0.0f));
float		fRotCounter = 0.0f;
Matrix4f	matProjection;
Sprite*		gTexture;
Mesh*		gMesh;

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

RenderContext::RenderContext(uint32_t iWidth, uint32_t iHeight, uint32_t iPixelWidth, uint32_t iPixelHeight)
: m_iWidth(iWidth)
, m_iHeight(iHeight)
, m_iPixelWidth(iPixelWidth)
, m_iPixelHeight(iPixelHeight)
{
	m_pFrameBuffer = new Sprite(iWidth, iHeight);
	m_pDepthBuffer = new float[iWidth * iHeight];
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

	gMesh = new Mesh("../Content/monkey2.obj");
	gTexture = new Sprite("../Content/cartoon.tga");

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
	Clear(Pixel(0, 0, 0, 0xFF), 1.0f);
	{
		fRotCounter += (iDeltaTimeMs / 1000.0f);
		Matrix4f matTranslation;
		matTranslation = matTranslation.initTranslation(0.0f, 0.0f, 3.0f);

		Matrix4f matRotation;
		matRotation = matRotation.initRotation(fRotCounter, fRotCounter, fRotCounter);
		
		Matrix4f matTransformation = matProjection.mul(matTranslation.mul(matRotation));
		
		DrawMesh(gMesh, matTransformation, gTexture);
	}
}

void RenderContext::DrawMesh(Mesh* pMesh, Matrix4f& matTransformation, Sprite* pTexture)
{
	for(int i = 0; i < pMesh->m_pMeshObjLoader->getNumFaces(); i++)
	{
		Vertex* v1 = pMesh->GetVertex(i * 3 + 0);
		Vertex* v2 = pMesh->GetVertex(i * 3 + 1);
		Vertex* v3 = pMesh->GetVertex(i * 3 + 2);
		
		FillTriangle(	v1->Transform(matTransformation),
						v2->Transform(matTransformation),
						v3->Transform(matTransformation),
						pTexture);
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

void RenderContext::Clear(Pixel p, float fDepth)
{
	uint32_t iSize = m_iWidth * m_iHeight;
	int32_t iPixel = p.GetPixel();
	for (int32_t i = 0; i < iSize; i++)
	{
		m_pFrameBuffer->SetPixel(i%m_iWidth, i / m_iWidth, p);
		m_pDepthBuffer[i] = fDepth;
	}
}

void RenderContext::FillTriangle(Vertex v1, Vertex v2, Vertex v3, Sprite* pTexture)
{
	Matrix4f screenSpaceTransform;
	screenSpaceTransform.initScreenSpaceTransform(GetWidth() / 2, GetHeight() / 2);

	Vertex vMinYVert = v1.Transform(screenSpaceTransform).PerspectiveDivide();
	Vertex vMidYVert = v2.Transform(screenSpaceTransform).PerspectiveDivide();
	Vertex vMaxYVert = v3.Transform(screenSpaceTransform).PerspectiveDivide();

	// Check to see if a triangle is facing the camera
	if (vMinYVert.TriangleAreaTimesTwo(vMaxYVert, vMidYVert) >= 0)
	{
		return;
	}

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
	float fDepthXStep = (eRight->GetDepth() - eLeft->GetDepth()) / fXDist;

	float fTexCoordX = eLeft->GetTexCoordX() + fTexCoordXXStep * fXPreStep;
	float fTexCoordY = eLeft->GetTexCoordY() + fTexCoordYXStep * fXPreStep;
	float fOneOverZ = eLeft->GetOneOverZ() + fOneOverZXStep * fXPreStep;
	float fDepth = eLeft->GetDepth() + fDepthXStep * fXPreStep;

	for (int i = xMin; i < xMax; i++)
	{
		if (fDepth < m_pDepthBuffer[i + j*m_iWidth])
		{
			m_pDepthBuffer[i + j*m_iWidth] = fDepth;

			float z = 1.0f / fOneOverZ;
			int srcX = (int)((fTexCoordX * z) * (float)(pTexture->GetWidth() - 1) + 0.5f);
			int srcY = (int)((fTexCoordY * z) * (float)(pTexture->GetHeight() - 1) + 0.5f);

			CopyPixel(i, j, srcX, srcY, pTexture);

			fOneOverZ += pGradients->GetOneOverZXStep();
			fTexCoordX += pGradients->GetTexCoordXXStep();
			fTexCoordY += pGradients->GetTexCoordYXStep();
			fDepth += pGradients->GetDepthXStep();
		}
	}
}

RenderContext::~RenderContext()
{

}