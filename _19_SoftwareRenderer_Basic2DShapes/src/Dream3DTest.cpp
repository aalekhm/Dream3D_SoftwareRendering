#include "Dream3DTest.h"
#include "Common/CCString.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Shape2D.h"

Dream3DTest		engine;

Shape2D*	gShape2D;

Camera*		gCamera;

Sprite*		gBrickTexture;
Sprite*		gBrickTexture2;
Sprite*		gCartoon;

Mesh*		gMonkeyMesh;
Transform	gMonkeyTransform;
Mesh*		gTerrainMesh;
Transform	gTerrainTransform;

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
	m_fRenderCallback = std::bind(&Dream3DTest::onRender, this, std::placeholders::_1);
	m_pGraphics = new RenderContext(EngineManager::getWidth(), EngineManager::getHeight(), EngineManager::getPixelWidth(), EngineManager::getPixelHeight(), &m_fRenderCallback);

	initWorld();
}

void Dream3DTest::initWorld()
{
	gMonkeyMesh = new Mesh("../Content/smoothMonkey2.obj");
	gMonkeyTransform = Transform(Vector4f(0, 0.0f, 3.0f));
	
	gTerrainMesh = new Mesh("../Content/terrain2.obj");
	gTerrainTransform = Transform(Vector4f(0, -1.0f, 0.0f));

	gBrickTexture = new Sprite("../Content/bricks.tga");
	gBrickTexture2 = new Sprite("../Content/bricks2.tga");
	gCartoon = new Sprite("../Content/cartoon.tga");

	gCamera = new Camera(	Matrix4f().initPerspective(	(float)(70.0f * (3.142f / 180.0f)),
					   									(float)EngineManager::getWidth() /(float)EngineManager::getHeight(),
														0.1f,
														1000.0f));

	std::vector<Shape2DVertex> vTriVertices;
	{
		// Clockwise winding
		vTriVertices.push_back(Shape2DVertex(Vector4f(-50, -50, 0, 1),	Vector4f(0.0, 0.0, 0, 1), Vector4f(1.0, 0.0, 0.0, 0.5f)));
		vTriVertices.push_back(Shape2DVertex(Vector4f(50, -50, 0, 1),	Vector4f(0.0, 1.0, 0, 1), Vector4f(0.0, 1.0, 0.0, 0.5f)));
		vTriVertices.push_back(Shape2DVertex(Vector4f(50, 50, 0, 1),	Vector4f(1.0, 0.0, 0, 1), Vector4f(0.0, 0.0, 1.0, 0.5f)));
		vTriVertices.push_back(Shape2DVertex(Vector4f(-50, 50, 0, 1),	Vector4f(1.0, 1.0, 0, 1), Vector4f(1.0, 0.0, 1.0, 0.5f)));
	}
	gShape2D = new Shape2D(vTriVertices, gCartoon);
	gShape2D->SetPosition(Vector4f(100, 100, 0));
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

	float fDeltaMs = iDeltaTimeMs / 1000.0f;
	// 3D
	{
		m_pGraphics->SetAmbientLightIntensity(0.2f);

		gCamera->update(fDeltaMs);
		Matrix4f vp = gCamera->getViewProjection();
	
		gMonkeyTransform = gMonkeyTransform.rotate(Quaternion(Vector4f(0, 1, 0), fDeltaMs));
	
		gTerrainMesh->Draw(m_pGraphics, vp, gTerrainTransform.getTransformation(), gBrickTexture);
		gMonkeyMesh->Draw(m_pGraphics, vp, gMonkeyTransform.getTransformation(), gBrickTexture2);
	}

	// 2D
	{
		m_pGraphics->SetAmbientLightIntensity(1.0f);
	
		gShape2D->Update(m_pGraphics, fDeltaMs);
	
		m_pGraphics->FillTriangle2D(	Vertex(	Vector4f(240, 100, 0),
												Vector4f(0, 0, 0, 0),
												Vector4f(0, 0, -1, 0),
												Vector4f(0, 0, 1, 0.5)),
										Vertex(	Vector4f(290, 200, 0),
												Vector4f(0, 0, 0, 0),
												Vector4f(0, 0, -1, 0),
												Vector4f(0, 1, 0, 0.5)),
										Vertex(	Vector4f(190, 200, 0),
												Vector4f(0, 0, 0, 0),
												Vector4f(0, 0, -1, 0),
												Vector4f(1, 0, 0, 0.5)),
										nullptr);
	}
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