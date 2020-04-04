#include "Engine/Console2D.h"
#include "Engine/TGA.h"
#include <assert.h>
#include "Engine/EngineManager.h"

Console2D* Console2D::m_pInstance = nullptr;

Console2D* Console2D::getInstance()
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new Console2D();
	}

	return m_pInstance;
}

Console2D::Console2D()
: m_VertexBuffer(nullptr)
, m_ClipRect(nullptr)
, m_ColorUVTable(nullptr)
{
	init();
}

void Console2D::init()
{
	assert(loadTexture("../Content/core.tga", m_iGLTexture, &m_iTexW, &m_iTexH));

	m_ClipRect = new RectF(0, 0, EngineManager::getInstance()->getWidth(), EngineManager::getInstance()->getHeight());

	m_ColorUVTable = new ColorUV[9];
	m_ColorUVTable[COLOR_BLACK].u = 675;	m_ColorUVTable[COLOR_BLACK].v = 55;
	m_ColorUVTable[COLOR_WHITE].u = 675;	m_ColorUVTable[COLOR_WHITE].v = 65;
	m_ColorUVTable[COLOR_RED].u = 675;		m_ColorUVTable[COLOR_RED].v = 149;//98;
	m_ColorUVTable[COLOR_GREEN].u = 675;	m_ColorUVTable[COLOR_GREEN].v = 161;//87;
	m_ColorUVTable[COLOR_BLUE].u = 675;		m_ColorUVTable[COLOR_BLUE].v = 173;//75;
	m_ColorUVTable[COLOR_GREY].u = 57;		m_ColorUVTable[COLOR_GREY].v = 8;
	m_ColorUVTable[COLOR_PINK].u = 675;		m_ColorUVTable[COLOR_PINK].v = 111;
	m_ColorUVTable[COLOR_CYAN].u = 675;		m_ColorUVTable[COLOR_CYAN].v = 123;
	m_ColorUVTable[COLOR_YELLOW].u = 675;	m_ColorUVTable[COLOR_YELLOW].v = 135;

	m_VertexBuffer = new VertexT2F_C4UB_V3F[SPR_MAX * 4];
}

void Console2D::drawStringFont(const char* cStr, int x, int y, int anchor)
{
	int strWidth = CHARACTER_WIDTH * (strlen(cStr) + 1);
	int TOTAL_ROWS	= 7;
	int COL = 0, ROW = 0, X = 0, Y = 0;

	int HLEFT = 0;
	int HCENTER = 1;
	int HRIGHT = 2;

	int xX = x;
	int yY = y;

	if(anchor == HCENTER)
		xX = xX - (strWidth/2);
	else
	if(anchor == HRIGHT)
		xX = xX - strWidth;

	for(int i = 0 ; i < strlen(cStr); i++) 
	{
		char c = cStr[i];

		if(c == '\r') 
		{
			continue;
		}

		if(c > ' ') 
		{
			COL = (c-33)%13;
			ROW = (c-33)/13;

			X = COL*CHARACTER_WIDTH;
			Y = ROW*CHARACTER_HEIGHT;

			drawFont(xX, yY, CHARACTER_WIDTH, CHARACTER_HEIGHT, X, Y);

			xX += (CHARACTER_WIDTH + 1);
		}
		else
		if(c == ' ' || c == '\t') 
		{
			xX += CHARACTER_WIDTH + 1;
		}
		else
		if(c == '\n') 
		{
			xX = x;
			yY += CHARACTER_HEIGHT + 3;
		}
	}
}

void Console2D::glSelectTexture(int id)
{
	glBindTexture(GL_TEXTURE_2D, m_iGLTexture);
}

void Console2D::updateConsole()
{
	m_iSpriteCount = 0;
}

void Console2D::renderConsole()
{
	setupOrthogonalProjection(m_ClipRect->X, m_ClipRect->Y, m_ClipRect->Width, m_ClipRect->Height);

	glEnable(GL_TEXTURE_2D);
	glSelectTexture(TEXTURE_CORE);
	
	glInterleavedArrays( GL_T2F_C4UB_V3F, 0, m_VertexBuffer);
	glDrawArrays( GL_QUADS, 0, m_iSpriteCount);

	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );

	glDisable(GL_TEXTURE_2D);
}

bool Console2D::loadTexture(char* sTexName, GLuint &texid, GLuint* texW, GLuint* texH)
{
	TGAImg Img;        // Image loader

	// Load our Texture
	if(Img.Load(sTexName) != IMG_OK)
		return false;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid); // Set our Tex handle as current

	// Specify filtering and edge actions
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

	// Create the texture
	if(Img.GetBPP() == 24) 
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 3, Img.GetWidth(), Img.GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, Img.GetImg());
	}
	else 
	if(Img.GetBPP() == 32)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, 4, Img.GetWidth(), Img.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Img.GetImg());
	}
	else
		return false;

	*texW = Img.GetWidth();
	*texH = Img.GetHeight();
	glDisable(GL_TEXTURE_2D);

	return true;
}

void Console2D::setClip(int x, int y, int w, int h)
{
	if (m_ClipRect == NULL)
		m_ClipRect = new RectF(x, y, w, h);

	m_ClipRect->X = x;
	m_ClipRect->Y = y;
	m_ClipRect->Width = w;
	m_ClipRect->Height = h;
}

void Console2D::setOrthogonalFrustum(float l, float r, float b, float t, float n, float f)
{
	matrixProjection.identity();
	matrixProjection[0] = 2 / (r - l);
	matrixProjection[3] = -(r + l) / (r - l);
	matrixProjection[5] = 2 / (t - b);
	matrixProjection[7] = -(t + b) / (t - b);
	matrixProjection[10] = -2 / (f - n);
	matrixProjection[11] = -(f + n) / (f - n);
}

void Console2D::setupOrthogonalProjection(double x, double y, double w, double h) 
{
	glDisable(GL_LIGHTING);

	setOrthogonalFrustum(x, x+w, y+h, y, 0.0, 1.0);
	// Setup and orthogonal, pixel-to-pixel projection matrix
	glMatrixMode(GL_PROJECTION);
	// copy projection matrix to OpenGL
	glLoadMatrixf(matrixProjection.getTranspose());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void Console2D::fillRect(u32 COLOR_ID, Rect* rect) {
	drawQuadU(TEXTURE_CORE, rect->X, rect->Y, rect->Width, rect->Height, m_ColorUVTable[COLOR_ID].u, m_ColorUVTable[COLOR_ID].v, 1, 1);
}

void Console2D::drawFont(int xX, int yY, int charW, int charH, int tX, int tY) {
	drawQuadU(TEXTURE_CORE, xX, yY, charW, charH, FONT_TEXTURE_START_X + tX, tY, charW, charH);
}

void Console2D::drawQuadU(	int TEXTURE_ID,
							float posX, float posY, float posW, float posH,
							float texX, float texY, float texW, float texH
) {
	GLuint TEX_WIDTH = m_iTexW;
	GLfloat tX = texX/TEX_WIDTH, tY = texY/TEX_WIDTH, tW = texW/TEX_WIDTH, tH = texH/TEX_WIDTH;

	RectF* drawRect = new RectF(posX, posY, posW, posH);
	RectF* posIntersectRect = new RectF(0, 0, posW, posH);
	RectF::Intersect(*posIntersectRect, *m_ClipRect, *drawRect);

	if(posIntersectRect->Width > 0 && posIntersectRect->Height > 0) 
	{
		////////// If IntersectRect width is less than texture width
		if(posIntersectRect->Width < texW) {
			texW = posIntersectRect->Width;
			tW = texW/TEX_WIDTH;

			////////// If posX < clipRectX, shift texture X equal to the diff in X
			if(posIntersectRect->X > posX) {
				texX += (posIntersectRect->X - posX);
				tX = texX/TEX_WIDTH;
			}
		}

		////////// If IntersectRect height is less than texture height
		if(posIntersectRect->Height < texH) {
			texH = posIntersectRect->Height;
			tH = texH/TEX_WIDTH;

			////////// If posY < clipRectY, shift texture Y equal to the diff in Y
			if(posIntersectRect->Y > posY) {
				texY += (posIntersectRect->Y - posY);
				tY = texY/TEX_WIDTH;
			}
		}

		//using VertexT2F_C4UB_V3F
		{	
			m_VertexBuffer[m_iSpriteCount+0].tu = tX;	m_VertexBuffer[m_iSpriteCount+0].tv = tY;
			m_VertexBuffer[m_iSpriteCount+0].color = 0xffffffff;
			m_VertexBuffer[m_iSpriteCount+0].vx = posIntersectRect->X;	m_VertexBuffer[m_iSpriteCount+0].vy = posIntersectRect->Y;	m_VertexBuffer[m_iSpriteCount+0].vz = 0.0f;
		}
		{	
			m_VertexBuffer[m_iSpriteCount+1].tu = tX;		m_VertexBuffer[m_iSpriteCount+1].tv = tY+tH;	
			m_VertexBuffer[m_iSpriteCount+1].color = 0xffffffff;
			m_VertexBuffer[m_iSpriteCount+1].vx = posIntersectRect->X;	m_VertexBuffer[m_iSpriteCount+1].vy = posIntersectRect->Y+posIntersectRect->Height; m_VertexBuffer[m_iSpriteCount+1].vz = 0.0f;
		}
		{	
			m_VertexBuffer[m_iSpriteCount+2].tu = tX+tW;	m_VertexBuffer[m_iSpriteCount+2].tv = tY+tH;	
			m_VertexBuffer[m_iSpriteCount+2].color = 0xffffffff;
			m_VertexBuffer[m_iSpriteCount+2].vx = posIntersectRect->X+posIntersectRect->Width;	m_VertexBuffer[m_iSpriteCount+2].vy = posIntersectRect->Y+posIntersectRect->Height; m_VertexBuffer[m_iSpriteCount+2].vz = 0.0f;
		}
		{	
			m_VertexBuffer[m_iSpriteCount+3].tu = tX+tW;	m_VertexBuffer[m_iSpriteCount+3].tv = tY;		
			m_VertexBuffer[m_iSpriteCount+3].color = 0xffffffff;
			m_VertexBuffer[m_iSpriteCount+3].vx = posIntersectRect->X+posIntersectRect->Width;	m_VertexBuffer[m_iSpriteCount+3].vy = posIntersectRect->Y; m_VertexBuffer[m_iSpriteCount+3].vz = 0.0f;
		}

		m_iSpriteCount += 4;
	}

	delete drawRect;
	delete posIntersectRect;
}

Console2D::~Console2D()
{
}