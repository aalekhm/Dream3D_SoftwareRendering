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

std::vector<Mesh*>		gBezierControlPoints3D;
std::vector<Transform*>	gBezierControlPoints3DTransform;
Mesh*					gBezierPoint3D;
Transform				gBezierPoint3DTransform;
std::vector<Vector4f>	gBeizerControlPoints2D;

Mesh*					gCubeMesh;
Transform				gCubeTransform;

float					gCounter;

bool					gShow3DTerrainMonkey = false;
bool					gShow3DBezierAnimation = true;
bool					gShow2DBezierAnimation = true;

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
	init3D();
	init2D();
}

void Dream3DTest::init3D()
{
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

	// Bezier 3D
	if(gShow3DBezierAnimation)
	{
		gBezierPoint3D = new Mesh("../Content/cube.obj");
		for (int32_t i = 0; i < 4; i++)
		{
			gBezierControlPoints3D.push_back(new Mesh("../Content/cube.obj"));

			Transform* t = new Transform(Vector4f(0, 0.0f, 0.0f), Quaternion(0, 0, 0, 0), Vector4f(0.05, 0.05f, 0.05f));
			gBezierControlPoints3DTransform.push_back(t);
		}

		gBezierControlPoints3DTransform[0] = new Transform(gBezierControlPoints3DTransform[0]->setPos(Vector4f(-0.5, 0.0f, 1.0f)));
		gBezierControlPoints3DTransform[1] = new Transform(gBezierControlPoints3DTransform[1]->setPos(Vector4f(0.0, 0.5f, 1.0f)));
		gBezierControlPoints3DTransform[2] = new Transform(gBezierControlPoints3DTransform[2]->setPos(Vector4f(0.5, 0.0f, 1.0f)));
		gBezierControlPoints3DTransform[3] = new Transform(gBezierControlPoints3DTransform[3]->setPos(Vector4f(0.5, 0.5f, 1.0f)));
		gBezierPoint3DTransform = Transform(Vector4f(0, 0.0f, 0.0f), Quaternion(0, 0, 0, 0), Vector4f(0.05, 0.05f, 0.05f));
	}

	//gCubeTransform = Transform(Vector4f(0, 0.5f, 3.0f));
	//gCubeTransform = gCubeTransform.setScale(Vector4f(0.1, 0.1f, 0.1f));

	gBrickTexture = new Sprite("../Content/bricks.tga");
	gBrickTexture2 = new Sprite("../Content/bricks2.tga");

	gCamera = new Camera(	Matrix4f().initPerspective(	(float)(70.0f * (3.142f / 180.0f)),
					   									(float)EngineManager::getWidth() /(float)EngineManager::getHeight(),
														0.1f,
														1000.0f));
}

void Dream3DTest::init2D()
{
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

	// Bezier 2D
	if(gShow2DBezierAnimation)
	{
		gCounter = 0.0f;
		gBeizerControlPoints2D.push_back(Vector4f(100, 100, 0, 0));
		gBeizerControlPoints2D.push_back(Vector4f(200,  10, 0, 0));
		gBeizerControlPoints2D.push_back(Vector4f(300,  10, 0, 0));
		gBeizerControlPoints2D.push_back(Vector4f(400, 100, 0, 0));
		//gBeizerControlPoints2D.push_back(Vector4f(300, 50, 0, 0));
	}
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
}

void Dream3DTest::render3D(uint32_t iDeltaTimeMs)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;
	m_pGraphics->SetAmbientLightIntensity(0.2f);

	gCamera->update(fDeltaMs);

	Matrix4f vp = gCamera->getViewProjection();

	// 3D Terrain Monkey
	if(gShow3DTerrainMonkey)
	{
		gMonkeyTransform = gMonkeyTransform.rotate(Quaternion(Vector4f(0, 1, 0), fDeltaMs));

		gTerrainMesh->Draw(m_pGraphics, vp, gTerrainTransform.getTransformation(), gBrickTexture);
		gMonkeyMesh->Draw(m_pGraphics, vp, gMonkeyTransform.getTransformation(), gBrickTexture2);
		//gCubeMesh->Draw(m_pGraphics, vp, gCubeTransform.getTransformation(), gBrickTexture);
	}

	// Bezier 3D Curve Animation
	if(gShow3DBezierAnimation)
	{
		std::vector<Vector4f> vControlPoints;
		for (int32_t i = 0; i < gBezierControlPoints3D.size(); i++)
		{
			gBezierControlPoints3D[i]->Draw(m_pGraphics, vp, gBezierControlPoints3DTransform[i]->getTransformation(), gBrickTexture);

			vControlPoints.push_back(gBezierControlPoints3DTransform[i]->getPos());
		}

		gCounter += fDeltaMs;
		float t = abs(sinf(gCounter));
		{
			Matrix4f mm = gBezierPoint3DTransform.getTransformation();
		
			Vector4f vBeizerPt = GetBeizerPoint(vControlPoints, t);
			mm.setTranslation(vBeizerPt.getX(), vBeizerPt.getY(), vBeizerPt.getZ());
			gBezierPoint3D->Draw(m_pGraphics, vp, mm, gBrickTexture);
		}
	}
}

void Dream3DTest::render2D(uint32_t iDeltaTimeMs)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;
	m_pGraphics->SetAmbientLightIntensity(1.0f);
	
	//gShape2D->Update(m_pGraphics, fDeltaMs);
	
	//m_pGraphics->FillTriangle2D(	Vertex(	Vector4f(240, 100, 0),
	//										Vector4f(0, 0, 0, 0),
	//										Vector4f(0, 0, -1, 0),
	//										Vector4f(0, 0, 1, 0.5)),
	//								Vertex(	Vector4f(290, 200, 0),
	//										Vector4f(0, 0, 0, 0),
	//										Vector4f(0, 0, -1, 0),
	//										Vector4f(0, 1, 0, 0.5)),
	//								Vertex(	Vector4f(190, 200, 0),
	//										Vector4f(0, 0, 0, 0),
	//										Vector4f(0, 0, -1, 0),
	//										Vector4f(1, 0, 0, 0.5)),
	//								nullptr);

	if (gShow2DBezierAnimation)
	{
		gCounter += fDeltaMs;
		float t = abs(sinf(gCounter)); // ==> (0 <= t <= 1)

		{
			// Curved path at every 't' interval of 0.05f
			for (float fT = 0.0f; fT < 1.0f; fT += 0.05f)
			{
				Vector4f vBeizerPt = GetBeizerPoint(gBeizerControlPoints2D, fT);
				m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 0, 255, 255), true);
			}

			// Animation Bezier Loci
			Vector4f vBeizerPt = GetBeizerPoint(gBeizerControlPoints2D, t);
			m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 255, 255, 255), true);
		}

		// Render Control Points
		for (int32_t i = 0; i < gBeizerControlPoints2D.size(); i++)
		{
			m_pGraphics->SetPixel(gBeizerControlPoints2D[i].getX(), gBeizerControlPoints2D[i].getY(), Pixel(255, 255, 255, 255), true);
		}
	}
}

Vector4f Dream3DTest::GetBeizerPoint(std::vector<Vector4f>& vControlPoints, float fTime)
{
	std::vector<Vector4f> vControlPointsInternal;
	for (int32_t i = 0; (i + 1) < vControlPoints.size(); i++)
	{
		Vector4f vP0 = vControlPoints[i];
		Vector4f vP1 = vControlPoints[i + 1];

		Vector4f vP01 = vP0.add((vP1.sub(vP0)).mul(fTime));
		vControlPointsInternal.push_back(vP01);
	}

	if (vControlPointsInternal.size() == 1)
	{
		return vControlPointsInternal[0];
	}
	else
	{
		return GetBeizerPoint(vControlPointsInternal, fTime);
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