#pragma once
#include <vector>
#include "Vector4f.h"
#include "BezierSurface3DUtils.h"
#include "Transform.h"
#include "Actor2D.h"
#include "Actor3D.h"

extern Sprite*			gBrickTexture;
extern Sprite*			gBrickTexture2;
extern Sprite*			gCartoon;
extern void				DrawMesh(RenderContext* pGraphics, Mesh* pMesh, Transform* pTransform, Matrix4f matViewProj, Sprite* pSprite);
extern void				printLine_(const char* lpszFormat, ...);

struct BezierSurface3D
{
	bool					m_bShowBezierSurface3D = false;
	Mesh*					m_pBezierSurfaceMesh;
	Transform				m_BezierSurfaceMeshTransform;
};

struct BezierAnimation2D
{
	bool					m_bShow2DBezierAnimation = false;
	std::vector<Vector4f>	m_vBeizerControlPoints2D;
};

struct BezierAnimation3D
{
	bool					m_bShow3DBezierAnimation = false;
	std::vector<Mesh*>		m_vBezierControlPoints3D;
	std::vector<Transform*>	m_vBezierControlPoints3DTransform;
	Mesh*					m_pBezierPoint3D;
	Transform				m_BezierPoint3DTransform;
};

struct BezierCurvePath2D
{
	bool					m_bShow2DBezierCurvePath2D = false;

	Actor2D*				m_pShapeSpaceShip2D;
	std::vector<Vector4f>	m_vBeizerCurvePathPoints2DList;
	std::vector<Vector4f>	m_vBeizerCurvePathPoints2DListGenerated;
};

struct BezierCurvePath3D
{
	bool						m_bShowBezierCurvePath3D = false;
	bool						m_bShowBezierCurvePath3D_CONSTANT_Y = false;

	std::vector<Mesh*>			m_vControlPointMeshes;
	std::vector<Mesh*>			m_vIntermediatePointMeshes;
	std::vector<Transform*>		m_vControlPointMeshesTransform;
	std::vector<Transform*>		m_vIntermediatePointMeshesTransform;
	std::vector<Vector4f>		m_vBeizerCurvePathPoints3DList;
	std::vector<Vector4f>		m_vBeizerCurvePathPoints3DListGenerated;

	Actor3D*					m_pShapeSpaceShip3D;
};

class BezierUtils
{
	public:
		static void				GenerateBezierCurvePoints(std::vector<Vector4f>& vInititalList, std::vector<Vector4f>& vFinalList);
		static Vector4f			GetBeizerPoint(std::vector<Vector4f>& vControlPoints, int32_t iStartPoint, int32_t iEndPoint, float fTime);
		static void				DrawBezierCurveFromPointList(	bool bIs2D,
																uint32_t iDeltaTimeMs,
																float fElapsedTimeMs,
																std::vector<Vector4f>& vBeizerCurvePathPointsOriginalList,
																std::vector<Vector4f>& vBeizerCurvePathPointsListGenerated);

		static void				init3D();
		static void				init2D();

		static void				render(int32_t iDeltaTimeMs);
		static void				render2D(int32_t iDeltaTimeMs);
		static void				render3D(int32_t iDeltaTimeMs);

		static void				init(RenderContext* pGraphics, Camera* pCamera);
		static RenderContext*	m_pGraphics;
		static Camera*			m_pCamera;
};