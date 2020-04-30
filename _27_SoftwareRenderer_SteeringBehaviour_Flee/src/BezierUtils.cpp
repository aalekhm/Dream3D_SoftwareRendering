#include "BezierUtils.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Utils2D.h"

RenderContext*	BezierUtils::m_pGraphics;
Camera*			BezierUtils::m_pCamera;

float					gElapsedTimeCounter;
float					gForceResetTimeCounter;

BezierSurface3D			gBezierSurface3D;
BezierAnimation2D		gBezierAnimation2D;
BezierAnimation3D		gBezierAnimation3D;
BezierCurvePath2D		gBezierCurvePath2D;
BezierCurvePath3D		gBezierCurvePath3D;

void BezierUtils::init(RenderContext* pGraphics, Camera* pCamera)
{
	m_pGraphics = pGraphics;
	m_pCamera = pCamera;

	gElapsedTimeCounter = gForceResetTimeCounter = 0.0f;

	init2D();
	init3D();
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

void BezierUtils::GenerateBezierCurvePoints(std::vector<Vector4f>& vInititalList, std::vector<Vector4f>& vFinalList)
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

	int32_t i = 0;
	for (Vector4f v : vInititalList)
	{
		printLine_("Init [%d] - %f, %f, %f", i++, v.x, v.y, v.z);
	}
	
	i = 0;
	for (Vector4f v : vFinalList)
	{
		printLine_("Final [%d] - %f, %f, %f", i++, v.x, v.y, v.z);
	}
}

Vector4f BezierUtils::GetBeizerPoint(std::vector<Vector4f>& vControlPoints, int32_t iStartPoint, int32_t iEndPoint, float fTime)
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

void BezierUtils::init2D()
{
	if (gBezierAnimation2D.m_bShow2DBezierAnimation)
	{
		gBezierAnimation2D.m_vBeizerControlPoints2D.push_back(Vector4f(100, 100, 0, 0));
		gBezierAnimation2D.m_vBeizerControlPoints2D.push_back(Vector4f(200, 10, 0, 0));
		gBezierAnimation2D.m_vBeizerControlPoints2D.push_back(Vector4f(300, 10, 0, 0));
		gBezierAnimation2D.m_vBeizerControlPoints2D.push_back(Vector4f(400, 100, 0, 0));
	}

	if (gBezierCurvePath2D.m_bShow2DBezierCurvePath2D)
	{
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(50,	200,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(100,	100,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(150,	200,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(200,	100,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(100,	 50,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(200,	 50,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(250,	100,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(300,	 50,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(350,	100,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(350,	250,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f(200,	250,	0, 0));
		gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList.push_back(Vector4f( 50,	200,	0, 0));

		BezierUtils::GenerateBezierCurvePoints(gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList, gBezierCurvePath2D.m_vBeizerCurvePathPoints2DListGenerated);

		gBezierCurvePath2D.m_pShapeSpaceShip2D = Utils2D::create2DSwastikaShip();
	}
}

void BezierUtils::init3D()
{
	if (gBezierSurface3D.m_bShowBezierSurface3D)
	{
		gBezierSurface3D.m_pBezierSurfaceMesh = BezierSurface3DUtils::GenerateBezierSurface();
		gBezierSurface3D.m_BezierSurfaceMeshTransform = Transform(Vector4f(0.0f, 0.0f, 0.0f), Quaternion(0, 0, 0, 0), Vector4f(2.0f, 2.0f, 2.0f));
	}

	if(gBezierAnimation3D.m_bShow3DBezierAnimation)
	{
		gBezierAnimation3D.m_pBezierPoint3D = new Mesh("../Content/cube.obj");
		for (int32_t i = 0; i < 4; i++)
		{
			gBezierAnimation3D.m_vBezierControlPoints3D.push_back(new Mesh("../Content/cube.obj"));

			Transform* t = new Transform(Vector4f(0, 0.0f, 0.0f), Quaternion(0, 0, 0, 0), Vector4f(0.05, 0.05f, 0.05f));
			gBezierAnimation3D.m_vBezierControlPoints3DTransform.push_back(t);
		}

		gBezierAnimation3D.m_vBezierControlPoints3DTransform[0] = new Transform(gBezierAnimation3D.m_vBezierControlPoints3DTransform[0]->setPos(Vector4f(-0.5, 0.0f, 1.0f)));
		gBezierAnimation3D.m_vBezierControlPoints3DTransform[1] = new Transform(gBezierAnimation3D.m_vBezierControlPoints3DTransform[1]->setPos(Vector4f(0.0, 0.5f, 1.0f)));
		gBezierAnimation3D.m_vBezierControlPoints3DTransform[2] = new Transform(gBezierAnimation3D.m_vBezierControlPoints3DTransform[2]->setPos(Vector4f(0.5, 0.0f, 1.0f)));
		gBezierAnimation3D.m_vBezierControlPoints3DTransform[3] = new Transform(gBezierAnimation3D.m_vBezierControlPoints3DTransform[3]->setPos(Vector4f(0.5, 0.5f, 1.0f)));
		gBezierAnimation3D.m_BezierPoint3DTransform = Transform(Vector4f(0, 0.0f, 0.0f), Quaternion(0, 0, 0, 0), Vector4f(0.05, 0.05f, 0.05f));
	}

	if (gBezierCurvePath3D.m_bShowBezierCurvePath3D)
	{
		gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.push_back(Vector4f(0.0, 1.0, 0.0, 1));
		gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.push_back(Vector4f(0.5, -0.5, -1.5, 1));
		gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.push_back(Vector4f(-1.0, 0.5, -2.5, 1));
		gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.push_back(Vector4f(1.0, -1.0, -0.5, 1));
		gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.push_back(Vector4f(0.5, -0.0, 1.5, 1));
		gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.push_back(Vector4f(-1.0, 0.5, -3.0, 1));
		gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.push_back(Vector4f(-2.0,	0.0,	-0.5,	1));

		BezierUtils::GenerateBezierCurvePoints(gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList, gBezierCurvePath3D.m_vBeizerCurvePathPoints3DListGenerated);

		gBezierCurvePath3D.m_pShapeSpaceShip3D = new Actor3D("../Content/Arrow.obj", "../Content/sp3bot.tga");
		gBezierCurvePath3D.m_pShapeSpaceShip3D->SetScale(Vector3(0.1f, 0.1f, 0.2f));
		gBezierCurvePath3D.m_pShapeSpaceShip3D->SetCamera(m_pCamera);

		// Control Points
		for (int32_t i = 0; i < gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList.size(); i++)
		{
			Mesh* pCubeMesh = new Mesh("../Content/cube.obj");
			gBezierCurvePath3D.m_vControlPointMeshes.push_back(pCubeMesh);

			Transform* pCubeTransform = new Transform(gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList[i],
														Quaternion(0, 0, 0, 1),
														Vector4f(0.1f, 0.1f, 0.1f)
													);
			gBezierCurvePath3D.m_vControlPointMeshesTransform.push_back(pCubeTransform);
		}

		// Points in-between the Control Points
		int32_t iStartControlPoint = 0, iEndControlPoint = 0;
		{
			int32_t iInterval = gBezierCurvePath3D.m_vBeizerCurvePathPoints3DListGenerated.size() / 3;
			float fTInterval = 1.0f / iInterval;
			Vector4f vBeizerPt;

			for (float fT = 0.05f; fT < 1.0f * iInterval; fT += 0.05f)
			{
				int32_t iStart = ((int32_t)fT % iInterval);
				iStartControlPoint = iStart * 3;
				iEndControlPoint = (iStart * 3) + 4;
				float fTT = ((fT / fTInterval) / iInterval) - iStart;			// 0 <= fTT <= 1.0f

				vBeizerPt = BezierUtils::GetBeizerPoint(gBezierCurvePath3D.m_vBeizerCurvePathPoints3DListGenerated, iStartControlPoint, iEndControlPoint, fTT);

				Mesh* pCubeMesh = new Mesh("../Content/cube.obj");
				gBezierCurvePath3D.m_vIntermediatePointMeshes.push_back(pCubeMesh);

				Transform* pCubeTransform = new Transform(	vBeizerPt,
															Quaternion(0, 0, 0, 1),
															Vector4f(0.025f, 0.025f, 0.025f)
				);

				gBezierCurvePath3D.m_vIntermediatePointMeshesTransform.push_back(pCubeTransform);
			}
		}
	}
}

void BezierUtils::render(int32_t iDeltaTimeMs)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;
	gElapsedTimeCounter += fDeltaMs;

	render2D(iDeltaTimeMs);
	render3D(iDeltaTimeMs);
}

void	BezierUtils::render2D(int32_t iDeltaTimeMs)
{
	if (gBezierAnimation2D.m_bShow2DBezierAnimation)
	{
		float t = abs(sinf(gElapsedTimeCounter)); // ==> (0 <= t <= 1)
		{
			// Curved path at every 't' interval of 0.05f
			for (float fT = 0.0f; fT < 1.0f; fT += 0.05f)
			{
				Vector4f vBeizerPt = BezierUtils::GetBeizerPoint(gBezierAnimation2D.m_vBeizerControlPoints2D, 0, gBezierAnimation2D.m_vBeizerControlPoints2D.size() - 1, fT);
				m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 0, 255, 255), true);
			}

			// Animation Bezier Loci
			Vector4f vBeizerPt = BezierUtils::GetBeizerPoint(gBezierAnimation2D.m_vBeizerControlPoints2D, 0, gBezierAnimation2D.m_vBeizerControlPoints2D.size() - 1, t);
			m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 255, 255, 255), true);
		}

		// Render Control Points
		for (int32_t i = 0; i < gBezierAnimation2D.m_vBeizerControlPoints2D.size(); i++)
		{
			m_pGraphics->SetPixel(gBezierAnimation2D.m_vBeizerControlPoints2D[i].getX(), gBezierAnimation2D.m_vBeizerControlPoints2D[i].getY(), Pixel(255, 255, 255, 255), true);
		}
	}

	if (gBezierCurvePath2D.m_bShow2DBezierCurvePath2D)
	{
		DrawBezierCurveFromPointList(	true, 
										iDeltaTimeMs, 
										(gElapsedTimeCounter - gForceResetTimeCounter), 
										gBezierCurvePath2D.m_vBeizerCurvePathPoints2DList, 
										gBezierCurvePath2D.m_vBeizerCurvePathPoints2DListGenerated);
	}
}

void BezierUtils::render3D(int32_t iDeltaTimeMs)
{
	Matrix4f matViewProj = m_pCamera->getViewProjection();

	if (gBezierSurface3D.m_bShowBezierSurface3D)
	{
		DrawMesh(m_pGraphics, gBezierSurface3D.m_pBezierSurfaceMesh, &gBezierSurface3D.m_BezierSurfaceMeshTransform, matViewProj, gCartoon);
	}

	// Bezier 3D Curve Animation
	if(gBezierAnimation3D.m_bShow3DBezierAnimation)
	{
		std::vector<Vector4f> vControlPoints;
		for (int32_t i = 0; i < gBezierAnimation3D.m_vBezierControlPoints3D.size(); i++)
		{
			DrawMesh(	m_pGraphics, 
						gBezierAnimation3D.m_vBezierControlPoints3D[i], 
						gBezierAnimation3D.m_vBezierControlPoints3DTransform[i], 
						matViewProj, 
						gBrickTexture);

			vControlPoints.push_back(gBezierAnimation3D.m_vBezierControlPoints3DTransform[i]->getPos());
		}

		float t = abs(sinf(gElapsedTimeCounter));
		{
			Vector4f vBeizerPt = BezierUtils::GetBeizerPoint(vControlPoints, 0, vControlPoints.size() - 1, t);
			gBezierAnimation3D.m_BezierPoint3DTransform = gBezierAnimation3D.m_BezierPoint3DTransform.setPos(vBeizerPt);

			DrawMesh(	m_pGraphics, 
						gBezierAnimation3D.m_pBezierPoint3D, 
						&gBezierAnimation3D.m_BezierPoint3DTransform, 
						matViewProj,
						gBrickTexture2);
		}
	}

	if (gBezierCurvePath3D.m_bShowBezierCurvePath3D)
	{
		DrawBezierCurveFromPointList(	false, 
										iDeltaTimeMs, 
										(gElapsedTimeCounter - gForceResetTimeCounter), 
										gBezierCurvePath3D.m_vBeizerCurvePathPoints3DList, 
										gBezierCurvePath3D.m_vBeizerCurvePathPoints3DListGenerated);
	}
}

float gLastTMs;
void BezierUtils::DrawBezierCurveFromPointList(	bool bIs2D,
														uint32_t iDeltaTimeMs,
														float fElapsedTimeMs,
														std::vector<Vector4f>& vBeizerCurvePathPointsOriginalList, 
														std::vector<Vector4f>& vBeizerCurvePathPointsListGenerated)
{
	// Projection Matrix for 3D
	Matrix4f matViewProj;
	if (!bIs2D)
	{
		matViewProj = m_pCamera->getViewProjection();
	}

	float fDeltaMs = iDeltaTimeMs * 0.001f;
	float fSlowFactor = 0.1f * 1;
	float t = abs(sinf(fElapsedTimeMs * fSlowFactor));	// ==> (0 <= t <= 1)

	int32_t iStartControlPoint = 0, iEndControlPoint = 0;
	{
		int32_t iInterval = vBeizerCurvePathPointsListGenerated.size() / 3;
		float fTInterval = 1.0f / iInterval;
		Vector4f vBeizerPt;

		// Curved path at every 't' interval of 0.05f
		int32_t iCount = 0;
		for (float fT = 0.05f; fT < 1.0f * iInterval; fT += 0.05f, iCount++)
		{
			if (bIs2D)
			{
				int32_t iStart = ((int32_t)fT % iInterval);
				iStartControlPoint = iStart * 3;
				iEndControlPoint = (iStart * 3) + 4;
				float fTT = ((fT / fTInterval) / iInterval) - iStart;			// 0 <= fTT <= 1.0f

				vBeizerPt = BezierUtils::GetBeizerPoint(vBeizerCurvePathPointsListGenerated, iStartControlPoint, iEndControlPoint, fTT);
				m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 0, 255, 255), true);
			}
			else
			{
				DrawMesh(	m_pGraphics, 
							gBezierCurvePath3D.m_vIntermediatePointMeshes[iCount], 
							gBezierCurvePath3D.m_vIntermediatePointMeshesTransform[iCount], 
							matViewProj, 
							gBrickTexture2);
			}
		}

		// Animation Bezier Loci
		int32_t iStart = t / fTInterval;
		int32_t iStartLast = gLastTMs / fTInterval;
		iStartControlPoint = iStart * 3;
		iEndControlPoint = (iStart * 3) + 4;
		if (iStartLast < iInterval && iStart < iInterval)
		{
			vBeizerPt = BezierUtils::GetBeizerPoint(vBeizerCurvePathPointsListGenerated, iStartControlPoint, iEndControlPoint, (t - iStart* fTInterval) * iInterval);
			Vector4f vLastBeizerPt = BezierUtils::GetBeizerPoint(vBeizerCurvePathPointsListGenerated, iStartLast * 3, (iStartLast * 3) + 4, (gLastTMs - iStartLast* fTInterval) * iInterval);
			{
				if (bIs2D)
				{
					m_pGraphics->SetPixel(vBeizerPt.getX(), vBeizerPt.getY(), Pixel(255, 255, 255, 255), true);

					gBezierCurvePath2D.m_pShapeSpaceShip2D->SetPosition(Vector3(vLastBeizerPt.x, vLastBeizerPt.y, 0));
					gBezierCurvePath2D.m_pShapeSpaceShip2D->LookAt(Vector3(vBeizerPt.x, vBeizerPt.y, 0));
					gBezierCurvePath2D.m_pShapeSpaceShip2D->Update(m_pGraphics, iDeltaTimeMs);
				}
				else
				{					
					gBezierCurvePath3D.m_pShapeSpaceShip3D->SetPosition(Vector3(vLastBeizerPt.x, vLastBeizerPt.y, vLastBeizerPt.z));

					if (gBezierCurvePath3D.m_bShowBezierCurvePath3D_CONSTANT_Y)
					{
						Vector4f vLookAt4f = ((vBeizerPt.sub(vLastBeizerPt)).normalized()).mul(1);
						vLookAt4f.add(vLastBeizerPt);
						//gBezierCurvePath3D.m_pShapeSpaceShip3D->LookAtConstantY(Vector3(vLookAt4f.x, vLookAt4f.y, vLookAt4f.z));
					}
					else
						gBezierCurvePath3D.m_pShapeSpaceShip3D->LookAt(Vector3(vBeizerPt.x, vBeizerPt.y, vBeizerPt.z));
					
					gBezierCurvePath3D.m_pShapeSpaceShip3D->Update(m_pGraphics, iDeltaTimeMs);
				}

				// Start again...
				if (vBeizerPt.equals(vBeizerCurvePathPointsListGenerated.at(vBeizerCurvePathPointsListGenerated.size() - 1)))
				{
					gForceResetTimeCounter = gElapsedTimeCounter;
				}
			}
		}
	}
	gLastTMs = t;

	// Render Control Points
	for (int32_t i = 0; i < vBeizerCurvePathPointsListGenerated.size(); i++)
	{
		for (int32_t j = 0; j < vBeizerCurvePathPointsOriginalList.size(); j++)
		{
			Vector4f vOriginal = vBeizerCurvePathPointsOriginalList[j];
			Vector4f vGenerated = vBeizerCurvePathPointsListGenerated[i];

			// Original Go-Through Points
			if (vOriginal.getX() == vGenerated.getX()
				&&
				vOriginal.getY() == vGenerated.getY()
			) {
				if (bIs2D)
				{
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
					DrawMesh(	m_pGraphics, 
								gBezierCurvePath3D.m_vControlPointMeshes[j],
								gBezierCurvePath3D.m_vControlPointMeshesTransform[j],
								matViewProj, 
								gBrickTexture);
				}
			}
			// Generated Control Points for the curve.
			else
			{
				Vector4f vStart = vBeizerCurvePathPointsListGenerated[iStartControlPoint];
				if (vStart.x == vOriginal.x
					&&
					vStart.y == vOriginal.y
					&&
					iStartControlPoint + 2 < vBeizerCurvePathPointsListGenerated.size() - 1
				) {
					Vector4f vStart_Plus1 = vBeizerCurvePathPointsListGenerated[iStartControlPoint + 1];
					{
						if (bIs2D)
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
					}

					Vector4f vStart_Plus2 = vBeizerCurvePathPointsListGenerated[iStartControlPoint + 2];
					{
						if (bIs2D)
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
}
