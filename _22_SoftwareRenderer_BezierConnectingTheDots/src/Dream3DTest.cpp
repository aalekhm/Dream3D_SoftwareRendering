#include "Dream3DTest.h"
#include "Common/CCString.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Shape2D.h"
#include "Actor2D.h"
#include "Vector4f.h"

Dream3DTest				engine;

Actor2D*				gShapeSpaceShip2D;

Camera*					gCamera;

Sprite*					gBrickTexture;
Sprite*					gBrickTexture2;
Sprite*					gCartoon;

Mesh*					gMonkeyMesh;
Transform				gMonkeyTransform;
Mesh*					gTerrainMesh;
Transform				gTerrainTransform;

std::vector<Mesh*>		gBezierControlPoints3D;
std::vector<Transform*>	gBezierControlPoints3DTransform;
Mesh*					gBezierPoint3D;
Transform				gBezierPoint3DTransform;
std::vector<Vector4f>	gBeizerControlPoints2D;

std::vector<Vector4f>	gBeizerCurvePathPoints2DList;
std::vector<Vector4f>	gBeizerCurvePathPoints2DListGenerated;

Mesh*					gCubeMesh;
Transform				gCubeTransform;

float					gElapsedTimeCounter;

Mesh*					gBezierSurfaceMesh;
Transform				gBezierSurfaceMeshTransform;

bool					gShow3DTerrainMonkey = false;
bool					gShow3DBezierAnimation = false;
bool					gShow2DBezierAnimation = false;
bool					gShow3DBezierSurface = false;
bool					gShow2DBezierCurvePath = true;

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

	if(gShow3DBezierSurface)
	{
		gBezierSurfaceMesh = GenerateBezierSurface();
		gBezierSurfaceMeshTransform = Transform(Vector4f(0.0f, 0.0f, 0.0f), Quaternion(0, 0, 0, 0), Vector4f(2.0f, 2.0f, 2.0f));
	}
}

void Dream3DTest::init2D()
{
	std::vector<Shape2DVertex> vTriVertices;
	{
		// Anticlockwise Clockwise winding
		vTriVertices.push_back(Shape2DVertex(Vector4f(-24, 24, 1, 1),	Vector4f(0.0, 1.0, 1, 1),	Vector4f(1.0, 0.0, 1.0, 0.0f)));
		vTriVertices.push_back(Shape2DVertex(Vector4f(24,  -25, 1, 1),	Vector4f(1.0, 0.0, 1, 1),	Vector4f(1.0, 0.0, 1.0, 0.0f)));
		vTriVertices.push_back(Shape2DVertex(Vector4f(-24,-25, 1, 1),	Vector4f(0.0, 0.0, 1, 1),	Vector4f(1.0, 0.0, 1.0, 0.0f)));

		vTriVertices.push_back(Shape2DVertex(Vector4f(-24, 24, 0, 1),	Vector4f(0.0, 1.0, 0, 1), Vector4f(1.0, 1.0, 0.0, 0.0f)));
		vTriVertices.push_back(Shape2DVertex(Vector4f(24,  25, 0, 1),	Vector4f(1.0, 1.0, 0, 1), Vector4f(1.0, 1.0, 0.0, 0.0f)));
		vTriVertices.push_back(Shape2DVertex(Vector4f(24, -25, 0, 1),	Vector4f(1.0, 0.0, 0, 1), Vector4f(1.0, 1.0, 0.0, 0.0f)));

		gShapeSpaceShip2D = new Actor2D(vTriVertices, "../Content/Swastik.tga");
		gShapeSpaceShip2D->SetPosition(Vector2(100, 100));
		gShapeSpaceShip2D->SetScale(Vector2(0.25f, 0.25f));
	}

	gElapsedTimeCounter = 0.0f;

	// Bezier 2D
	if(gShow2DBezierAnimation)
	{	
		gBeizerControlPoints2D.push_back(Vector4f(100, 100, 0, 0));
		gBeizerControlPoints2D.push_back(Vector4f(200,  10, 0, 0));
		gBeizerControlPoints2D.push_back(Vector4f(300,  10, 0, 0));
		gBeizerControlPoints2D.push_back(Vector4f(400, 100, 0, 0));
	}

	if (gShow2DBezierCurvePath)
	{
		gBeizerCurvePathPoints2DList.push_back(Vector4f(50,		200,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(100,	100,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(150,	200,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(200,	100,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(100,	 50,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(200,	 50,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(250,	100,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(300,	 50,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(350,	100,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(350,	250,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f(200,	250,	0, 0));
		gBeizerCurvePathPoints2DList.push_back(Vector4f( 50,	200,	0, 0));

		Generate2DBezierCurvePoints(gBeizerCurvePathPoints2DList, gBeizerCurvePathPoints2DListGenerated);
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

	float fDeltaMs = iDeltaTimeMs * 0.001f;
	gElapsedTimeCounter += fDeltaMs;

	//render3D(iDeltaTimeMs);
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

		float t = abs(sinf(gElapsedTimeCounter));
		{
			Matrix4f mm = gBezierPoint3DTransform.getTransformation();
		
			Vector4f vBeizerPt = GetBeizerPoint(vControlPoints, 0, vControlPoints.size() - 1, t);
			mm.setTranslation(vBeizerPt.getX(), vBeizerPt.getY(), vBeizerPt.getZ());
			gBezierPoint3D->Draw(m_pGraphics, vp, mm, gBrickTexture);
		}
	}
	
	if(gShow3DBezierSurface)
	{
		gBezierSurfaceMesh->Draw(m_pGraphics, vp, gBezierSurfaceMeshTransform.getTransformation(), gCartoon);
	}
}

void Dream3DTest::render2D(uint32_t iDeltaTimeMs)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;
	m_pGraphics->SetAmbientLightIntensity(1.0f);
	
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
		float t = abs(sinf(gElapsedTimeCounter)); // ==> (0 <= t <= 1)
		{
			// Curved path at every 't' interval of 0.05f
			for (float fT = 0.0f; fT < 1.0f; fT += 0.05f)
			{
				Vector4f vBeizerPt = GetBeizerPoint(gBeizerControlPoints2D, 0, gBeizerControlPoints2D.size() - 1, fT);
				m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 0, 255, 255), true);
			}

			// Animation Bezier Loci
			Vector4f vBeizerPt = GetBeizerPoint(gBeizerControlPoints2D, 0, gBeizerControlPoints2D.size() - 1, t);
			m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 255, 255, 255), true);
		}

		// Render Control Points
		for (int32_t i = 0; i < gBeizerControlPoints2D.size(); i++)
		{
			m_pGraphics->SetPixel(gBeizerControlPoints2D[i].getX(), gBeizerControlPoints2D[i].getY(), Pixel(255, 255, 255, 255), true);
		}
	}

	if (gShow2DBezierCurvePath)
	{
		Draw2DBezierCurveFromPointList(iDeltaTimeMs, gElapsedTimeCounter, gBeizerCurvePathPoints2DList, gBeizerCurvePathPoints2DListGenerated);
	}
}

float gLastTMs;

void Dream3DTest::Draw2DBezierCurveFromPointList(	uint32_t iDeltaTimeMs, 
													float fElapsedTimeMs,
													std::vector<Vector4f>& vBeizerCurvePathPoints2DOriginalList, 
													std::vector<Vector4f>& vBeizerCurvePathPoints2DListGenerated)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;
	float fSlowFactor = 0.1f;
	float t = abs(sinf(fElapsedTimeMs * fSlowFactor));	// ==> (0 <= t <= 1)

	int32_t iStartControlPoint = 0, iEndControlPoint = 0;
	{
		int32_t iInterval = vBeizerCurvePathPoints2DListGenerated.size() / 3;
		float fTInterval = 1.0f / iInterval;
		Vector4f vBeizerPt;

		// Curved path at every 't' interval of 0.05f
		for (float fT = 0.05f; fT < 1.0f * iInterval; fT += 0.05f)
		{
			int32_t iStart = ((int32_t)fT % iInterval);
			iStartControlPoint = iStart * 3;
			iEndControlPoint = (iStart * 3) + 4;
			float fTT = ((fT / fTInterval) / iInterval) - iStart;			// 0 <= fTT <= 1.0f

			vBeizerPt = GetBeizerPoint(vBeizerCurvePathPoints2DListGenerated, iStartControlPoint, iEndControlPoint, fTT);
			m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 0, 255, 255), true);
		}

		// Animation Bezier Loci
		int32_t iStart = t / fTInterval;
		int32_t iStartLast = gLastTMs / fTInterval;
		iStartControlPoint = iStart * 3;
		iEndControlPoint = (iStart * 3) + 4;
		if (iStartLast < iInterval && iStart < iInterval)
		{
			vBeizerPt = GetBeizerPoint(vBeizerCurvePathPoints2DListGenerated, iStartControlPoint, iEndControlPoint, (t - iStart* fTInterval) * iInterval);
			m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 255, 255, 255), true);

			Vector4f vLastBeizerPt = GetBeizerPoint(vBeizerCurvePathPoints2DListGenerated, iStartLast * 3, (iStartLast * 3) + 4, (gLastTMs - iStartLast* fTInterval) * iInterval);
			{
				Vector4f vLookAt4f = vLastBeizerPt.add((((vBeizerPt.sub(vLastBeizerPt)).normalized()).mul(10.0f)));

				gShapeSpaceShip2D->SetPosition(Vector2(vLastBeizerPt.x, vLastBeizerPt.y));
				gShapeSpaceShip2D->LookAt(Vector2(vBeizerPt.x, vBeizerPt.y));				
				gShapeSpaceShip2D->Update(m_pGraphics, iDeltaTimeMs);
			}
		}
	}
	gLastTMs = t;

	// Render Control Points
	for (int32_t i = 0; i < vBeizerCurvePathPoints2DListGenerated.size(); i++)
	{
		for (int32_t j = 0; j < vBeizerCurvePathPoints2DOriginalList.size(); j++)
		{
			Vector4f vOriginal = vBeizerCurvePathPoints2DOriginalList[j];
			Vector4f vGenerated = vBeizerCurvePathPoints2DListGenerated[i];

			if (vOriginal.getX() == vGenerated.getX()
				&&
				vOriginal.getY() == vGenerated.getY()
			) {
				m_pGraphics->SetPixel(vOriginal.getX() - 1, vOriginal.getY() - 1,	Pixel(0, 255, 255, 255), true);
				m_pGraphics->SetPixel(vOriginal.getX(),		vOriginal.getY() - 1,	Pixel(0, 255, 255, 255), true);
				m_pGraphics->SetPixel(vOriginal.getX() + 1, vOriginal.getY() - 1,	Pixel(0, 255, 255, 255), true);

				m_pGraphics->SetPixel(vOriginal.getX() - 1, vOriginal.getY(),		Pixel(0, 255, 255, 255), true);
				m_pGraphics->SetPixel(vOriginal.getX(),		vOriginal.getY(),		Pixel(0, 255, 255, 255), true);
				m_pGraphics->SetPixel(vOriginal.getX() + 1, vOriginal.getY(),		Pixel(0, 255, 255, 255), true);

				m_pGraphics->SetPixel(vOriginal.getX() - 1, vOriginal.getY() + 1,	Pixel(0, 255, 255, 255), true);
				m_pGraphics->SetPixel(vOriginal.getX(),		vOriginal.getY() + 1,	Pixel(0, 255, 255, 255), true);
				m_pGraphics->SetPixel(vOriginal.getX() + 1, vOriginal.getY() + 1,	Pixel(0, 255, 255, 255), true);
			}
			else
			{
				Vector4f vStart = vBeizerCurvePathPoints2DListGenerated[iStartControlPoint];
				if (vStart.x == vOriginal.x
					&&
					vStart.y == vOriginal.y
					&&
					iStartControlPoint + 2 < vBeizerCurvePathPoints2DListGenerated.size() - 1
				) {
					Vector4f vStart_Plus1 = vBeizerCurvePathPoints2DListGenerated[iStartControlPoint + 1];
					{
						m_pGraphics->SetPixel(vStart_Plus1.getX() - 1,	vStart_Plus1.getY() - 1,	Pixel(0, 255, 0, 255), true);
						m_pGraphics->SetPixel(vStart_Plus1.getX(),		vStart_Plus1.getY() - 1,	Pixel(0, 255, 0, 255), true);
						m_pGraphics->SetPixel(vStart_Plus1.getX() + 1,	vStart_Plus1.getY() - 1,	Pixel(0, 255, 0, 255), true);

						m_pGraphics->SetPixel(vStart_Plus1.getX() - 1,	vStart_Plus1.getY(),		Pixel(0, 255, 0, 255), true);
						m_pGraphics->SetPixel(vStart_Plus1.getX(),		vStart_Plus1.getY(),		Pixel(0, 255, 0, 255), true);
						m_pGraphics->SetPixel(vStart_Plus1.getX() + 1,	vStart_Plus1.getY(),		Pixel(0, 255, 0, 255), true);

						m_pGraphics->SetPixel(vStart_Plus1.getX() - 1,	vStart_Plus1.getY() + 1,	Pixel(0, 255, 0, 255), true);
						m_pGraphics->SetPixel(vStart_Plus1.getX(),		vStart_Plus1.getY() + 1,	Pixel(0, 255, 0, 255), true);
						m_pGraphics->SetPixel(vStart_Plus1.getX() + 1,	vStart_Plus1.getY() + 1,	Pixel(0, 255, 0, 255), true);
					}

					Vector4f vStart_Plus2 = vBeizerCurvePathPoints2DListGenerated[iStartControlPoint + 2];
					{
						m_pGraphics->SetPixel(vStart_Plus2.getX() - 1,	vStart_Plus2.getY() - 1,	Pixel(255, 0, 255, 255), true);
						m_pGraphics->SetPixel(vStart_Plus2.getX(),		vStart_Plus2.getY() - 1,	Pixel(255, 0, 255, 255), true);
						m_pGraphics->SetPixel(vStart_Plus2.getX() + 1,	vStart_Plus2.getY() - 1,	Pixel(255, 0, 255, 255), true);

						m_pGraphics->SetPixel(vStart_Plus2.getX() - 1,	vStart_Plus2.getY(),		Pixel(255, 0, 255, 255), true);
						m_pGraphics->SetPixel(vStart_Plus2.getX(),		vStart_Plus2.getY(),		Pixel(255, 0, 255, 255), true);
						m_pGraphics->SetPixel(vStart_Plus2.getX() + 1,	vStart_Plus2.getY(),		Pixel(255, 0, 255, 255), true);

						m_pGraphics->SetPixel(vStart_Plus2.getX() - 1,	vStart_Plus2.getY() + 1,	Pixel(255, 0, 255, 255), true);
						m_pGraphics->SetPixel(vStart_Plus2.getX(),		vStart_Plus2.getY() + 1,	Pixel(255, 0, 255, 255), true);
						m_pGraphics->SetPixel(vStart_Plus2.getX() + 1,	vStart_Plus2.getY() + 1,	Pixel(255, 0, 255, 255), true);
					}
				}
			}
		}
	}
}

/*
	Idea is to Traverse through the Control Points
	@ = Original Control Points (A, B, C, D) - N control points
	The Bezier Curve should pass through all @'s

									(*)            (0)
									  ,         ,
									   ,    ,
									B	@
								   .     .
						       .          .
					       .               .					D	@
					A	@					.					 .
											 .				 .
											  .			 .
											   .     .
											C	@

			For The Bezier curve that starts from 'A' & pass through 'B',
			'A' & 'B' should be the 1st & Last control points of the Bezier curve.
			We generate 4 intermediate Control Points as follows:
			#1	-	Pt1 :	'A'

			#2	-	Pt2 :	If (Start Point - A)
								then 'A'
							Else
								-Vector(A - B)
								Negative vector from B to A.
								Normalize it.
								Multiply it with some XFactor(say 5 or 10) &
								Add it to B(Point3) to get a point away from 'B' in the direction of the generated vector.
								That becomes our (0) in the figure above.

			#3	-	Pt3 :	If (Last point - D)
								then 'D'
							Else
								-Vector(C - B)
								Negative vector from B to C.
								Normalize it
								Multiply it with some XFactor(say 5 or 10) & 
								Add it to B(Point3) to get a point away from 'B' in the direction of the generated vector.
								That becomes our (*) from the figure above.

			#4	-	Pt4 :	B

			#5	-	Add these 4 points to a vector.
			#6	-	Take the next control point 'B' from the original list to generate a Bezier Curve from 'B' to 'C'
					&
					Continue the same process from #1 - #5 till you reach (N - 1)th control point.

			NB: 'XFactor' forces the curvature towards the target point (less the value, straighter is the curve).
			This can be applied to 3D points to generate a smooth curve that passes through all the points in a list.
*/

void Dream3DTest::Generate2DBezierCurvePoints(std::vector<Vector4f>& vInititalList, std::vector<Vector4f>& vFinalList)
{
	Vector4f v1 = vInititalList.at(0);
	vFinalList.push_back(v1);

	Vector4f v23_;

	for (int32_t i = 0; i < vInititalList.size() - 2; i++)
	{
		v1 = vInititalList.at(i);
		if(i == 0)
			vFinalList.push_back(v1);
		else
		{
			v23_ = v23_.mul(-1.0f);
			Vector4f vTemp = v1.add(v23_);
			vFinalList.push_back(vTemp);
		}

		Vector4f v2 = vInititalList.at(i + 1);
		Vector4f v3 = vInititalList.at(i + 2);
		
		Vector4f _v23 = v3.sub(v2);
		Vector4f _v12 = v2.sub(v1);
		float fHalfLength = _v12.length() * 0.5f;
		v23_ = (((_v23.normalized()).mul(-1.0f)).mul(fHalfLength));
		Vector4f v23 = v2.add(v23_);

		vFinalList.push_back(v23);
		vFinalList.push_back(v2);
	}

	// Last point.
	vFinalList.push_back(vInititalList.at(vInititalList.size() - 2));
	vFinalList.push_back(vInititalList.at(vInititalList.size() - 1));
	vFinalList.push_back(vInititalList.at(vInititalList.size() - 1));
}

Vector4f Dream3DTest::GetBeizerPoint(std::vector<Vector4f>& vControlPoints, int32_t iStartPoint, int32_t iEndPoint, float fTime)
{
	std::vector<Vector4f> vControlPointsInternal;
	for (int32_t i = iStartPoint; i < iEndPoint - 1; i++)
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
		return GetBeizerPoint(vControlPointsInternal, 0, vControlPointsInternal.size() , fTime);
	}
}

/*
#define NI 5
#define NJ 4
Vector4f inp[NI + 1][NJ + 1];
#define RESOLUTIONI 10*NI
#define RESOLUTIONJ 10*NJ
Vector4f outp[RESOLUTIONI][RESOLUTIONJ];

Mesh* Dream3DTest::GenerateBezierSurfaceOrig()
{
	int i, j, ki, kj;
	double mui, muj, bi, bj;

	// Create a random surface
	srand(1111);
	for (i = 0; i <= NI; i++) 
	{
		for (j = 0; j <= NJ; j++) 
		{
			inp[i][j].x = i;
			inp[i][j].y = j;
			inp[i][j].z = (rand() % 100) / 50.0 - 1;
		}
	}

	for (i = 0; i < RESOLUTIONI; i++) 
	{
		mui = i / (double)(RESOLUTIONI - 1);
		for (j = 0; j < RESOLUTIONJ; j++) 
		{
			muj = j / (double)(RESOLUTIONJ - 1);
			outp[i][j].x = 0;
			outp[i][j].y = 0;
			outp[i][j].z = 0;
			for (ki = 0; ki <= NI; ki++) 
			{
				bi = BezierBlend(ki, mui, NI);
				for (kj = 0; kj <= NJ; kj++) 
				{
					bj = BezierBlend(kj, muj, NJ);
					outp[i][j].x += (inp[ki][kj].x * bi * bj);
					outp[i][j].y += (inp[ki][kj].y * bi * bj);
					outp[i][j].z += (inp[ki][kj].z * bi * bj);
				}
			}
		}
	}

	//printLine_("LIST");
	//
	// Display the surface, in this case in OOGL format for GeomView
	//printLine_("{ = CQUAD");
	//for (i = 0; i<RESOLUTIONI - 1; i++) 
	//{
	//	for (j = 0; j<RESOLUTIONJ - 1; j++) 
	//	{
	//		printLine_("%g %g %g 1 1 1 1",	outp[i][j].x, outp[i][j].y, outp[i][j].z);
	//		printLine_("%g %g %g 1 1 1 1",	outp[i][j + 1].x, outp[i][j + 1].y, outp[i][j + 1].z);
	//		printLine_("%g %g %g 1 1 1 1",	outp[i + 1][j + 1].x, outp[i + 1][j + 1].y, outp[i + 1][j + 1].z);
	//		printLine_("%g %g %g 1 1 1 1",	outp[i + 1][j].x, outp[i + 1][j].y, outp[i + 1][j].z);
	//	}
	//}
	//printLine_("}\n");
	//
	// Control point polygon
	//for (i = 0; i<NI; i++)
	//{
	//	for (j = 0; j<NJ; j++)
	//	{
	//		printLine_("{ = SKEL 4 1");
	//		printLine_("%g %g %g", inp[i][j].x, inp[i][j].y, inp[i][j].z);
	//		printLine_("%g %g %g", inp[i][j + 1].x, inp[i][j + 1].y, inp[i][j + 1].z);
	//		printLine_("%g %g %g", inp[i + 1][j + 1].x, inp[i + 1][j + 1].y, inp[i + 1][j + 1].z);
	//		printLine_("%g %g %g", inp[i + 1][j].x, inp[i + 1][j].y, inp[i + 1][j].z);
	//		printLine_("5 0 1 2 3 0");
	//		printLine_("}\n");
	//	}
	//}

	std::vector<Vertex> vVertices;
	for (i = 0; i < RESOLUTIONI - 1; i++)
	{
		for (j = 0; j < RESOLUTIONJ - 1; j++)
		{
			Vector4f v1Pos = Vector4f(outp[i][j].x, outp[i][j].y, outp[i][j].z, 1.0f);
			Vector4f v3Pos = Vector4f(outp[i + 1][j].x, outp[i + 1][j].y, outp[i + 1][j].z, 1.0f);
			Vector4f v2Pos = Vector4f(outp[i][j + 1].x, outp[i][j + 1].y, outp[i][j + 1].z, 1.0f);

			Vector4f vector1 = v3Pos.sub(v2Pos);
			Vector4f vector2 = v3Pos.sub(v1Pos);
			Vector4f vNormal = vector1.cross(vector2).normalized();

			Vertex v1(	v1Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v2(	v2Pos,
						Vector4f(0.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v3(	v3Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v1);
			vVertices.push_back(v2);
			vVertices.push_back(v3);

			Vector4f v4Pos = Vector4f(outp[i + 1][j].x, outp[i + 1][j].y, outp[i + 1][j].z, 1.0f);
			Vector4f v6Pos = Vector4f(outp[i + 1][j + 1].x, outp[i + 1][j + 1].y, outp[i + 1][j + 1].z, 1.0f);
			Vector4f v5Pos = Vector4f(outp[i][j + 1].x, outp[i][j + 1].y, outp[i][j + 1].z, 1.0f);

			vector1 = v6Pos.sub(v5Pos);
			vector2 = v6Pos.sub(v4Pos);
			vNormal = vector1.cross(vector2).normalized();

			Vertex v4(	v4Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v5(	v5Pos,
						Vector4f(1.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v6(	v6Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v4);
			vVertices.push_back(v5);
			vVertices.push_back(v6);
		}
	}

	Mesh* pBezierSurface = new Mesh(vVertices);
	return pBezierSurface;
}
//*/

///*
#define CP_X 5
#define CP_Y 4
#define RESOLUTIONY 10*CP_Y
#define RESOLUTIONX 10*CP_X
Mesh* Dream3DTest::GenerateBezierSurface()
{
	Vector4f inp[CP_Y + 1][CP_X + 1];
	Vector4f outp[RESOLUTIONY][RESOLUTIONX];

	int y, x, ky, kx;
	double muy, mux, bx, by;

	// Create a random surface
	// Creating Control Points at pos (x, y, randomZ)
	// randomZ gives a curved effect on the surface.
	srand(1111);
	for (y = 0; y <= CP_Y; y++) 
	{
		for (x = 0; x <= CP_X; x++) 
		{
			inp[y][x].x = x;
			inp[y][x].y = (rand() % 100) / 50.0 - 1;
			inp[y][x].z = y;
		}
	}

	// 
	// x, y position on the surface act as 't' (0 <= t <= 1) in both 'u'(muy) & 'v'(mux) direction.
	// The influence of each control point (kx or ky) out of the total control points(CP_X, CP_Y) at a particular 't' (mux & muy) is calculated in the BezierBlend()
	//		& added as a cumulative blend factor for a particular x, y position.
	//
	for (y = 0; y < RESOLUTIONY; y++) 
	{
		muy = y / (double)(RESOLUTIONY - 1);
		for (x = 0; x < RESOLUTIONX; x++) 
		{
			mux = x / (double)(RESOLUTIONX - 1);
			outp[y][x].x = 0;
			outp[y][x].y = 0;
			outp[y][x].z = 0;
			for (ky = 0; ky <= CP_Y; ky++) 
			{
				by = BezierBlend(ky, muy, CP_Y);
				for (kx = 0; kx <= CP_X; kx++) 
				{
					bx = BezierBlend(kx, mux, CP_X);
					outp[y][x].x += (inp[ky][kx].x * bx * by);
					outp[y][x].y += (inp[ky][kx].y * bx * by);
					outp[y][x].z += (inp[ky][kx].z * bx * by);
				}
			}
		}
	}

	std::vector<Vertex> vVertices;
	for (y = 0; y < RESOLUTIONY - 1; y++)
	{
		for (x = 0; x < RESOLUTIONX - 1; x++)
		{																			//			X					  X + 1
			Vector4f v1Pos = Vector4f(	outp[y]		[x].x,							//		  v1Pos					v2Pos
										outp[y]		[x].y, 							//	  Y		@*********************@
										outp[y]		[x].z, 1.0f);					//			*				   *
			Vector4f v2Pos = Vector4f(	outp[y]		[x + 1].x, 						//			*				*
										outp[y]		[x + 1].y,						//			*			 *
										outp[y]		[x + 1].z, 1.0f);				//			*		  *
			Vector4f v3Pos = Vector4f(	outp[y + 1]	[x].x, 							//			*	   *
										outp[y + 1]	[x].y, 							//			*   *
										outp[y + 1]	[x].z, 1.0f);					//	Y + 1	@*
																					//		  v3Pos
			Vector4f vector1 = v2Pos.sub(v3Pos);
			Vector4f vector2 = v2Pos.sub(v1Pos);
			Vector4f vNormal = vector2.cross(vector1).normalized();

			Vertex v1(	v1Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v2(	v2Pos,
						Vector4f(0.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v3(	v3Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v1);
			vVertices.push_back(v2);
			vVertices.push_back(v3);
																					//			X					X + 1
			Vector4f v4Pos = Vector4f(	outp[y + 1]	[x].x,							//		  						v5Pos
										outp[y + 1]	[x].y, 							//	  Y							  @
										outp[y + 1]	[x].z, 1.0f);					//							   *  *
			Vector4f v5Pos = Vector4f(	outp[y]		[x + 1].x, 						//							*	  *
										outp[y]		[x + 1].y, 						//						 *		  *
										outp[y]		[x + 1].z, 1.0f); 				//					  *			  *
			Vector4f v6Pos = Vector4f(	outp[y + 1]	[x + 1].x, 						//				   *			  *
										outp[y + 1]	[x + 1].y, 						//			    *				  *
										outp[y + 1]	[x + 1].z, 1.0f);				//	Y + 1	 @*********************@
																					//		  v4Pos					 v6Pos
			vector1 = v5Pos.sub(v4Pos);
			vector2 = v5Pos.sub(v6Pos);
			vNormal = vector1.cross(vector2).normalized();

			Vertex v4(	v4Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v5(	v5Pos,
						Vector4f(1.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v6(	v6Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v4);
			vVertices.push_back(v5);
			vVertices.push_back(v6);
		}
	}

	Mesh* pBezierSurface = new Mesh(vVertices);
	return pBezierSurface;
}
//*/

/*
Mesh* Dream3DTest::GenerateBezierSurfaceMy()
{
	int32_t _CP_Y = 5;
	int32_t _CP_X = 4;
	int32_t _RESOLUTIONY = 10 * _CP_Y;
	int32_t _RESOLUTIONX = 10 * _CP_X;

	Vector4f* inp = new Vector4f[(_CP_Y + 1 ) * (_CP_X + 1)];
	Vector4f* outp = new Vector4f[_RESOLUTIONY * _RESOLUTIONX];

	int y, x, ky, kx;
	double muy, mux, bx, by;

	// Create a random surface
	srand(1111);
	for (y = 0; y <= _CP_Y; y++) 
	{
		for (x = 0; x <= _CP_X; x++) 
		{
			inp[y + x * _CP_Y].x = x;
			inp[y + x * _CP_Y].y = y;
			inp[y + x * _CP_Y].z = (rand() % 100) / 50.0 - 1;
		}
	}

	for (y = 0; y < _RESOLUTIONY; y++) 
	{
		muy = y / (double)(_RESOLUTIONY - 1);
		for (x = 0; x < _RESOLUTIONX; x++) 
		{
			mux = x / (double)(_RESOLUTIONX - 1);
			outp[y + x * _RESOLUTIONY].x = 0;
			outp[y + x * _RESOLUTIONY].y = 0;
			outp[y + x * _RESOLUTIONY].z = 0;
			for (ky = 0; ky <= _CP_Y; ky++) 
			{
				by = BezierBlend(ky, muy, _CP_Y);
				for (kx = 0; kx <= _CP_X; kx++) 
				{
					bx = BezierBlend(kx, mux, _CP_X);
					outp[y + x * _RESOLUTIONY].x += (inp[ky + kx * _CP_Y].x * bx * by);
					outp[y + x * _RESOLUTIONY].y += (inp[ky + kx * _CP_Y].y * bx * by);
					outp[y + x * _RESOLUTIONY].z += (inp[ky + kx * _CP_Y].z * bx * by);
				}
			}
		}
	}

	std::vector<Vertex> vVertices;
	for (y = 0; y < _RESOLUTIONY - 1; y++)
	{
		for (x = 0; x < _RESOLUTIONX - 1; x++)												//			X					  X + 1
		{																					//		  v1Pos					v2Pos
			Vector4f v1Pos = Vector4f(	outp[y + x * _RESOLUTIONY].x,						//	  Y		@*********************@
										outp[y + x * _RESOLUTIONY].y,						//			*				   *
										outp[y + x * _RESOLUTIONY].z, 1.0f);				//			*				*
			Vector4f v3Pos = Vector4f(	outp[(y + 1) + x * _RESOLUTIONY].x,					//			*			 *
										outp[(y + 1) + x * _RESOLUTIONY].y,					//			*		  *
										outp[(y + 1) + x * _RESOLUTIONY].z, 1.0f);			//			*	   *
			Vector4f v2Pos = Vector4f(	outp[y + (x + 1) * _RESOLUTIONY].x,					//			*   *
										outp[y + (x + 1) * _RESOLUTIONY].y,					//	Y + 1	@*
										outp[y + (x + 1) * _RESOLUTIONY].z, 1.0f);			//		  v3Pos

			Vector4f vector1 = v3Pos.sub(v2Pos);
			Vector4f vector2 = v3Pos.sub(v1Pos);
			Vector4f vNormal = vector1.cross(vector2).normalized();

			Vertex v1(	v1Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v2(	v2Pos,
						Vector4f(0.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v3(	v3Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v1);
			vVertices.push_back(v2);
			vVertices.push_back(v3);
																								//			X					X + 1
			Vector4f v4Pos = Vector4f(	outp[(y + 1) + x * _RESOLUTIONY].x,						//		  						v5Pos
										outp[(y + 1) + x * _RESOLUTIONY].y, 					//	  Y							  @
										outp[(y + 1) + x * _RESOLUTIONY].z, 1.0f);				//							   *  *
			Vector4f v6Pos = Vector4f(	outp[(y + 1) + (x + 1) * _RESOLUTIONY].x, 				//							*	  *
										outp[(y + 1) + (x + 1) * _RESOLUTIONY].y, 				//						 *		  *
										outp[(y + 1) + (x + 1) * _RESOLUTIONY].z, 1.0f);		//					  *			  *
			Vector4f v5Pos = Vector4f(	outp[y + (x + 1) * _RESOLUTIONY].x, 					//				   *			  *
										outp[y + (x + 1) * _RESOLUTIONY].y, 					//			    *				  *
										outp[y + (x + 1) * _RESOLUTIONY].z, 1.0f);				//	Y + 1	 @*********************@
																								//		  v4Pos					 v6Pos
			vector1 = v6Pos.sub(v5Pos);
			vector2 = v6Pos.sub(v4Pos);
			vNormal = vector1.cross(vector2).normalized();

			Vertex v4(	v4Pos,
						Vector4f(0.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v5(	v5Pos,
						Vector4f(1.0f, 0.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());

			Vertex v6(	v6Pos,
						Vector4f(1.0f, 1.0f, 0.0f, 0.0f), 
						vNormal,
						Vector4f());
			vVertices.push_back(v4);
			vVertices.push_back(v5);
			vVertices.push_back(v6);
		}
	}

	Mesh* pBezierSurface = new Mesh(vVertices);
	return pBezierSurface;
}
*/
// Courtesy : http://paulbourke.net/geometry/bezier/
//
//				N!		   k		 N-k
//	blend =	----------- * t   * (1-t)
//			 k! (N-k)!
//
// where :=
//		N = Number of control points
//		k = kth control point
//		t = 0 <= t <= 1
//
// Calculate the influence of 'k'th control point at time 't', where total number of control points is 'n'.
//
double Dream3DTest::BezierBlend(int k, float t, int n)
{
	int N,kn,nkn;
	double blend=1;

	N = n;
	kn = k;
	nkn = n - k;

	while (N >= 1) 
	{
		blend *= N;							// N!
		N--;

		if (kn > 1)
		{
			blend /= (double)kn;			// __1__
			kn--;							//   k!
		}

		if (nkn > 1)
		{
			blend /= (double)nkn;			// ___1___
			nkn--;							//  (N-k)!
		}
	}

	if (k > 0)
	{										//  K
		blend *= pow(t,(double)k);			// t
	}

	if (n-k > 0)
	{										//		 N-k
		blend *= pow(1-t,(double)(n-k));	// (1 - u)
	}

	return(blend);
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