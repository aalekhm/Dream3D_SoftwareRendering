#pragma once
#include "stdint.h"
#include "Vector4f.h"
#include <vector>

class Actor2D;
class Actor3D;
class RenderContext;
class Camera;

struct Seek2D
{
	bool					m_bEnable;
	Actor2D*				m_pSeeker;
	Actor2D*				m_pTarget;
	bool					m_bInitialized;
};

struct Seek3D
{
	bool					m_bEnable;
	Actor3D*				m_pSeeker;
	Actor3D*				m_pTarget;

	std::vector<Vector3>	m_vTarget3DPositions;
	int32_t					m_iSeekPos;
	bool					m_bInitialized;
};

struct Arrive2D
{
	bool					m_bEnable;
	Actor2D*				m_pSeeker;
	Actor2D*				m_pTarget;
	bool					m_bInitialized;
};

struct Arrive3D
{
	bool					m_bEnable;
	Actor3D*				m_pSeeker;
	Actor3D*				m_pTarget;

	std::vector<Vector3>	m_vTarget3DPositions;
	int32_t					m_iArrivePos;
	bool					m_bInitialized;
};

struct Flee2D
{
	bool					m_bEnable;
	Actor2D*				m_pSeeker;
	Actor2D*				m_pTarget;
	bool					m_bInitialized;
};

struct Flee3D
{
	bool					m_bEnable;
	Actor3D*				m_pSeeker;
	Actor3D*				m_pTarget;
	Actor3D*				m_pBox;

	int32_t					m_iFleePos;
	bool					m_bInitialized;
};

struct Wander2D
{
	bool					m_bEnable;
	Actor2D*				m_pSeeker;
	bool					m_bInitialized;
};

struct Wander3D
{
	bool					m_bEnable;
	Actor3D*				m_pSeeker;
	bool					m_bInitialized;
};

struct Pursuit2D
{
	bool					m_bEnable;
	Actor2D*				m_pSeeker;
	Actor2D*				m_pTarget;
	bool					m_bInitialized;
};

struct Pursuit3D
{
	bool					m_bEnable;
	Actor3D*				m_pSeeker;
	Actor3D*				m_pTarget;
	bool					m_bInitialized;
};

class SteerBehaviourUtils
{
	public:
		static void				init(RenderContext* pGraphics, Camera* pCamera);
		static void				render(int32_t iDeltaTimeMs);

		static void				onMouseUpEx(int mCode, int x, int y);
		static void				keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch);
	protected:
	private:
		static void				initContent();
		static void				init2D();
		static void				init3D();

		static void				render2D(int32_t iDeltaTimeMs);
		static void				render3D(int32_t iDeltaTimeMs);

		static void				check2DBounds(Actor2D* pActor2D);
		static void				check3DBounds(Actor3D* pActor3D);

		static RenderContext*	m_pGraphics;
		static Camera*			m_pCamera;

		static bool				m_b2DToggle;

		static uint32_t			m_iElapsedTimeMs;
};