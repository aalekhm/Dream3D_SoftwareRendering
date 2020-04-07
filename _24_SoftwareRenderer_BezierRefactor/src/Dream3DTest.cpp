#include "Dream3DTest.h"
#include "Common/CCString.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Shape2D.h"
#include "Actor2D.h"
#include "Actor3D.h"
#include "Vector4f.h"
#include "BezierUtils.h"

#define RAD2DEG(__angleInRadians__) (180 * __angleInRadians__ / 3.141593f)
#define DEG2RAD(__angleInDegrees__) (3.141593f * __angleInDegrees__ / 180)
const float EPSILON = 0.00001f;

Dream3DTest				engine;

Sprite*					gBrickTexture;
Sprite*					gBrickTexture2;
Sprite*					gCartoon;
Sprite*					gCoreTga;

bool					gShow3DTerrainMonkey = false;
Mesh*					gMonkeyMesh;
Transform				gMonkeyTransform;

Mesh*					gTerrainMesh;
Transform				gTerrainTransform;

Mesh*					gCubeMesh;
Transform				gCubeTransform;

void printLine_(const char* lpszFormat, ...)
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

void DrawMesh(RenderContext* pGraphics, Mesh* pMesh, Transform* pTransform, Matrix4f matViewProj, Sprite* pSprite)
{
	pMesh->Draw(pGraphics, matViewProj, pTransform->getTransformation(), pSprite);
}

Dream3DTest::Dream3DTest()
{
	setViewport(640, 480);
	setPixelSize(1, 1);
}

Dream3DTest::~Dream3DTest()
{
}

void Dream3DTest::initialize()
{
	m_fRenderCallback = std::bind(&Dream3DTest::onRender, this, std::placeholders::_1);
	m_pGraphics = new RenderContext(EngineManager::getWidth(), EngineManager::getHeight(), EngineManager::getPixelWidth(), EngineManager::getPixelHeight(), &m_fRenderCallback);
	Sprite* pFontSprite = new Sprite("../Content/core.tga");
	m_pGraphics->SetFont(pFontSprite);

	initWorld();
}

void Dream3DTest::initWorld()
{
	init3D();
	init2D();

	BezierUtils::init(m_pGraphics, m_pCamera);
}

void Dream3DTest::init3D()
{
	m_pCamera = new Camera(	Matrix4f().initPerspective(	(float)(70.0f * (3.142f / 180.0f)),
					   								(float)EngineManager::getWidth() /(float)EngineManager::getHeight(),
													0.1f,
													1000.0f));
	m_pCamera->move(Vector4f(0, 1, 0), 1);
	m_pCamera->move(Vector4f(0, 0, -5), 1);

	gCartoon = new Sprite("../Content/cartoon.tga");

	// 3D Terrain Monkey
	if(gShow3DTerrainMonkey)
	{
		gMonkeyMesh = new Mesh("../Content/smoothMonkey2.obj");
		gMonkeyTransform = Transform(Vector4f(0, 0.0f, 3.0f));

		gTerrainMesh = new Mesh("../Content/terrain2.obj");
		gTerrainTransform = Transform(Vector4f(0, -1.0f, 0.0f));
	}
	
	//gCubeMesh = new Mesh("../Content/cube.obj");

	//gCubeTransform = Transform(Vector4f(0, 0.5f, 3.0f));
	//gCubeTransform = gCubeTransform.setScale(Vector4f(0.1, 0.1f, 0.1f));

	gBrickTexture = new Sprite("../Content/bricks.tga");
	gBrickTexture2 = new Sprite("../Content/bricks2.tga");
}

void Dream3DTest::init2D()
{
	gCoreTga = new Sprite("../Content/core.tga");
}

void Dream3DTest::update(float elapsedTime)
{

}

void Dream3DTest::render(float elapsedTime)
{
	m_pGraphics->Render();
}

void Dream3DTest::onRender(uint32_t iDeltaTimeMs)
{
	m_pGraphics->Clear(Pixel(0, 0, 0, 0xFF), FLT_MAX);

	render3D(iDeltaTimeMs);
	render2D(iDeltaTimeMs);

	BezierUtils::render(iDeltaTimeMs);
}

void Dream3DTest::render3D(uint32_t iDeltaTimeMs)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;
	m_pGraphics->SetAmbientLightIntensity(1.2f);

	m_pCamera->update(fDeltaMs);

	// 3D Terrain Monkey
	if(gShow3DTerrainMonkey)
	{
		Matrix4f viewProjMat = m_pCamera->getViewProjection();

		gMonkeyTransform = gMonkeyTransform.rotate(Quaternion(Vector4f(0, 1, 0), fDeltaMs));

		DrawMesh(m_pGraphics, gTerrainMesh, &gTerrainTransform, viewProjMat, gBrickTexture);
		DrawMesh(m_pGraphics, gMonkeyMesh, &gMonkeyTransform, viewProjMat, gBrickTexture2);
		//DrawMesh(m_pGraphics, gCubeMesh, &gCubeTransform, viewProjMat, gBrickTexture);
	}	
}

void Dream3DTest::render2D(uint32_t iDeltaTimeMs)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;
	m_pGraphics->SetAmbientLightIntensity(1.0f);
	
	//m_pGraphics->FillQuad2D(	240, 100, 128, 128, 
	//							0, 0, 1024, 1024,
	//							gCoreTga, 0.5f);
	//
	//m_pGraphics->FillTriangle2D(	Vertex(	Vector4f(240, 100, 0, 1),
	//										Vector4f(0, 0, 0, 1),
	//										Vector4f(0, 0, -1, 0),
	//										Vector4f(0, 0, 1, 0.5)),
	//								Vertex(	Vector4f(290, 200, 0, 1),
	//										Vector4f(0, 1, 0, 1),
	//										Vector4f(0, 0, -1, 0),
	//										Vector4f(0, 1, 0, 0.5)),
	//								Vertex(	Vector4f(190, 200, 0, 1),
	//										Vector4f(1, 0, 0, 1),
	//										Vector4f(0, 0, -1, 0),
	//										Vector4f(1, 0, 0, 0.5)),
	//								gCoreTga);

	DrawStatusBar();
}

void Dream3DTest::DrawStatusBar()
{
	EngineManager* pEngineManager = getInstance();

	int32_t iStatusBarHeight = 50;
	int32_t iTop = pEngineManager->getHeight() - iStatusBarHeight;
	int32_t iStartX = 0;
	int32_t iStartY = iTop + 2;

	m_pGraphics->FillQuad2D(0, iTop, pEngineManager->getWidth(), iStatusBarHeight, Vector4f(0, 0, 0, 1));
	m_pGraphics->DrawString(m_sStatusBasrString.c_str(), iStartX, iStartY, 0);

	m_pGraphics->FillQuad2D(0, iTop, pEngineManager->getWidth(), 1, Vector4f(1, 0, 1, 1));
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