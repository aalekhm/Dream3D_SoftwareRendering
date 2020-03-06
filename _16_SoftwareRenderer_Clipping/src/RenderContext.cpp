#include "RenderContext.h"
#include "Sprite.h"
#include <GL/glew.h>
#include "Matrix4f.h"
#include "Common/CCString.h"
#include <limits>

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
		char sBuffer[1024];
		memset(sBuffer, 0, 1024);
		_vsnprintf(sBuffer, 1023, lpszFormat, args);

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

	gMesh = new Mesh("../Content/monkey0.obj");
	gTexture = new Sprite("../Content/cartoon.tga");

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
	Clear(Pixel(0, 0, 0, 0xFF), FLT_MAX);
	{
		fRotCounter += (iDeltaTimeMs / 1000.0f);
		Matrix4f matTranslation;
		matTranslation = matTranslation.initTranslation(0.0f, 0.0f, 13.0f - 13 * (float)sinf(fRotCounter));

		Matrix4f matRotation;
		matRotation = matRotation.initRotation(fRotCounter, fRotCounter, fRotCounter);
		
		Matrix4f matTransformation = matProjection.mul(matTranslation.mul(matRotation));
		
		gMesh->Draw(this, matTransformation, gTexture);
	}
}

void RenderContext::DrawTriangle(Vertex v1, Vertex v2, Vertex v3, Sprite* pBitmapTexture)
{
	bool bIsV1Inside = v1.IsInsideViewFrustum();
	bool bIsV2Inside = v2.IsInsideViewFrustum();
	bool bIsV3Inside = v3.IsInsideViewFrustum();

	if(bIsV1Inside && bIsV2Inside && bIsV3Inside)
	{
		FillTriangle(v1, v2, v3, pBitmapTexture);
		return;
	}

	if(!bIsV1Inside && !bIsV2Inside && !bIsV3Inside)
	{
		return;
	}

	std::vector<Vertex> vVertices;
	std::vector<Vertex> vAuxillaryList;
		
	vVertices.push_back(v1);
	vVertices.push_back(v2);
	vVertices.push_back(v3);
		
	/* Check if any vertices are inside & add them to the list.
		* If some are outside, calculate the intersection & add them to the list
		* & render
		*/
	if(	ClipPolygonAxis(vVertices, vAuxillaryList, AXIS_COMPONENT::AXIS_X/* Check for limits on X-Axis*/)
		&&
		ClipPolygonAxis(vVertices, vAuxillaryList, AXIS_COMPONENT::AXIS_Y/* Check for limits on Y-Axis*/)
		&&
		ClipPolygonAxis(vVertices, vAuxillaryList, AXIS_COMPONENT::AXIS_Z/* Check for limits on Z-Axis*/))
	{
		Vertex vInitialVertex = vVertices.at(0);

		for (int i = 1; i < vVertices.size() - 1; i++)
		{
			FillTriangle(vInitialVertex, vVertices.at(i), vVertices.at(i + 1), pBitmapTexture);
		}
	}
}

bool RenderContext::ClipPolygonAxis(std::vector<Vertex>& vVertices, std::vector<Vertex>& vAuxillaryList, AXIS_COMPONENT eComponentIndex)
{
	ClipPolygonComponent(	vVertices, 
							eComponentIndex, 
							1.0f 				/*Check for +ve limit*/, 
							vAuxillaryList);
	vVertices.clear();
		
	// All the vertices are outside.
	if(vAuxillaryList.empty())
	{
		return false;
	}

	ClipPolygonComponent(	vAuxillaryList, 
							eComponentIndex, 
							-1.0f 			/*Check for -ve limit*/, 
							vVertices);
	vAuxillaryList.clear();

	return !vVertices.empty();
}

/*
	* 		|-------------------------------|
	* 		|								|
	* 		|								|
	* 		|								|
	* 		|						 v1.	|
	* 		|						   |\	|
	* 		|						   | \	|
	* 		|						   |  \	|
	* 		|						   |   \|
	* 	-1	|..............0................\-----------> point of intersection on +ve X-Axis
	* 		|						   |	|\.v2
	*  		|						   |	|/
	*  		|					       |	/-----------> point of intersection on +ve X-Axis
	*  		|						   |   /|
	*  		|						   v3./	|
	*  		|                          | /  |
	*		|						   ./   |
	*  		|								|
	*  		|-------------------------------|
	*  clipPolygonComponent calculates the exact intersection point on the desired axis
	*  & adds it to the results list of vertices.
	*/ 
void RenderContext::ClipPolygonComponent(std::vector<Vertex>& vVertices, AXIS_COMPONENT eComponentIndex, float fComponentFactor, std::vector<Vertex>& vResult)
{
	Vertex vPreviousVertex = vVertices.at(vVertices.size() - 1);
	float fPreviousComponent = vPreviousVertex.Get(eComponentIndex) * fComponentFactor;
	bool bPreviousInside = (fPreviousComponent <= vPreviousVertex.GetPosition().getW());

	std::vector<Vertex>::iterator it = vVertices.begin() + 1;
	while(it != vVertices.end())
	{
		Vertex vCurrentVertex = *it;
		float fCurrentComponent = vCurrentVertex.Get(eComponentIndex) * fComponentFactor;
		bool bCurrentInside = (fComponentFactor <= vCurrentVertex.GetPosition().getW());

		// If one of the vertices is outside(false),
		// calculate new position on the axis.
		//if(!bCurrentInside || !bPreviousInside)
		if (bCurrentInside ^ bPreviousInside)
		{
			/*
				* 		-1			A       B   +1    C
				* 		 |..........*.......*....|....*
				*                               D
				* 
				*  	So, we have to lerp from B->C so thet the result is +1.
				* 			1 		= B(1 - LerpAmt) + C * LerpAmt;
				* 			1 		= B - B*LerpAmt + C*LerpAmt + (LerpAmt - LerpAmt);
				* 			1 - B 	= LerpAmt - LerpAmt*B - LerpAmt + C*LerpAmt;
				* 			1 - B 	= LerpAmt(1 - B) - LerpAmt(1 + C);
				* 			1 - B	= LerpAmt((1 - B) - (1 + C));
				* 			LerpAmt	= (1 - B) / ((1 - B) - (1 + C))
				* 
				* 		Now, Divide by ZERO from perspective ?
				* 		Simple solution, clip before perspective divide !
				* 			-1 <= 	  Xp    <= 1
				* 			-1 <=  Xp / W	<= 1
				* 			-W <= 	  Xp	<= W
				* 		,So clipping is against W & -W, NOT 1 & -1, reason being we are clipping before perspective divide.
				* 		,hence our LerpAmt equation now becomes :
				* 			LerpAmt	= (Wb - B) / ((Wb - B) - (Wc - C));
				*/
			
			float lerpAmt = (vPreviousVertex.GetPosition().getW() - fPreviousComponent) 
							/
							(	(vPreviousVertex.GetPosition().getW() - fPreviousComponent) 
								- 
								(vCurrentVertex.GetPosition().getW() - fCurrentComponent)
							);

			Vertex vV = vPreviousVertex.Lerp(vCurrentVertex, lerpAmt);
			vResult.push_back(vV);
		}

		if(bCurrentInside)
		{
			vResult.push_back(vCurrentVertex);
		}

		vPreviousVertex 		= vCurrentVertex;
		fPreviousComponent 		= fCurrentComponent;
		bPreviousInside 		= bCurrentInside;

		it++;
		if (it == vVertices.end() - 1)
		{
			break;
		}
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
	float fArea = vMinYVert.TriangleAreaTimesTwo(vMaxYVert, vMidYVert);
	if (fArea >= 0)
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

	fArea = vMinYVert.TriangleAreaTimesTwo(vMaxYVert, vMidYVert);
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