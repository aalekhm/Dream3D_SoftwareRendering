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

Mesh*					gBezierSurfaceMesh;
Transform				gBezierSurfaceMeshTransform;

bool					gShow3DTerrainMonkey = false;
bool					gShow3DBezierAnimation = false;
bool					gShow2DBezierAnimation = false;
bool					gShow3DBezierSurface = true;

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
	
	if(gShow3DBezierSurface)
	{
		gBezierSurfaceMesh->Draw(m_pGraphics, vp, gBezierSurfaceMeshTransform.getTransformation(), gCartoon);
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
			inp[y][x].x = y;
			inp[y][x].y = x;
			inp[y][x].z = (rand() % 100) / 50.0 - 1;
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
			Vector4f v1Pos = Vector4f(	outp[y][x].x,								//		  v1Pos					v2Pos
										outp[y][x].y, 								//	  Y		@*********************@
										outp[y][x].z, 1.0f);						//			*				   *
			Vector4f v2Pos = Vector4f(	outp[y][x + 1].x, 							//			*				*
										outp[y][x + 1].y,							//			*			 *
										outp[y][x + 1].z, 1.0f);					//			*		  *
			Vector4f v3Pos = Vector4f(	outp[y + 1][x].x, 							//			*	   *
										outp[y + 1][x].y, 							//			*   *
										outp[y + 1][x].z, 1.0f);					//	Y + 1	@*
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
			Vector4f v4Pos = Vector4f(	outp[y + 1][x].x,							//		  						v5Pos
										outp[y + 1][x].y, 							//	  Y							  @
										outp[y + 1][x].z, 1.0f);					//							   *  *
			Vector4f v5Pos = Vector4f(	outp[y][x + 1].x, 							//							*	  *
										outp[y][x + 1].y, 							//						 *		  *
										outp[y][x + 1].z, 1.0f); 					//					  *			  *
			Vector4f v6Pos = Vector4f(	outp[y + 1][x + 1].x, 						//				   *			  *
										outp[y + 1][x + 1].y, 						//			    *				  *
										outp[y + 1][x + 1].z, 1.0f);				//	Y + 1	 @*********************@
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