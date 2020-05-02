#include "SteerBehaviourUtils.h"
#include "Utils2D.h"
#include "ActorSteerBehaviour.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Actor3D.h"
#include "Engine/EngineManager.h"

RenderContext*	SteerBehaviourUtils::m_pGraphics;
Camera*			SteerBehaviourUtils::m_pCamera;
bool			SteerBehaviourUtils::m_b2DToggle = true;
uint32_t		SteerBehaviourUtils::m_iElapsedTimeMs = 0;

Seek2D		gSeek2D;
Seek3D		gSeek3D;
Arrive2D	gArrive2D;
Arrive3D	gArrive3D;
Flee2D		gFlee2D;
Flee3D		gFlee3D;
Wander2D	gWander2D;
Wander3D	gWander3D;

void SteerBehaviourUtils::init(RenderContext* pGraphics, Camera* pCamera)
{
	m_pGraphics = pGraphics;
	m_pCamera = pCamera;
	m_iElapsedTimeMs = 0;
	{
		gSeek2D.m_bEnable = false;
		gSeek3D.m_bEnable = false;
		gArrive2D.m_bEnable = false;
		gArrive3D.m_bEnable = false;
		gFlee2D.m_bEnable = false;
		gFlee3D.m_bEnable = false;
		gWander2D.m_bEnable = true;
		gWander3D.m_bEnable = false;
	}

	initContent();
}

void SteerBehaviourUtils::initContent()
{
	init2D();
	init3D();
}

void SteerBehaviourUtils::render(int32_t iDeltaTimeMs)
{
	m_iElapsedTimeMs += iDeltaTimeMs;

	render2D(iDeltaTimeMs);
	render3D(iDeltaTimeMs);
}

void SteerBehaviourUtils::init2D()
{
	if (gSeek2D.m_bEnable && !gSeek2D.m_bInitialized)
	{
		gSeek2D.m_bInitialized = true;

		gSeek2D.m_pTarget = Utils2D::create2DSwastikaShip();
		{
			gSeek2D.m_pTarget->SetPosition(Vector3(100, 100, 0));
			gSeek2D.m_pTarget->SetScale(Vector3(0.25, 0.25, 0.25));
		}

		gSeek2D.m_pSeeker = Utils2D::create2DSwastikaShip();
		{
			gSeek2D.m_pSeeker->SetPosition(Vector3(50, 50, 0));
			gSeek2D.m_pSeeker->SetScale(Vector3(0.25, 0.25, 0.25));
			gSeek2D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
			{
				ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gSeek2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
				if (pActorSteerBehaviour != nullptr)
				{
					pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::SEEK);
					pActorSteerBehaviour->SetTarget(ESteerBehaviour::SEEK, gSeek2D.m_pTarget);
				}
			}
		}
	}

	if (gArrive2D.m_bEnable && !gArrive2D.m_bInitialized)
	{
		gArrive2D.m_bInitialized = true;

		gArrive2D.m_pTarget = Utils2D::create2DSwastikaShip();
		{
			gArrive2D.m_pTarget->SetPosition(Vector3(100, 100, 0));
			gArrive2D.m_pTarget->SetScale(Vector3(0.25, 0.25, 0.25));
		}

		gArrive2D.m_pSeeker = Utils2D::create2DSwastikaShip();
		{
			gArrive2D.m_pSeeker->SetPosition(Vector3(50, 50, 0));
			gArrive2D.m_pSeeker->SetScale(Vector3(0.25, 0.25, 0.25));
			gArrive2D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
			{
				ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gArrive2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
				if (pActorSteerBehaviour != nullptr)
				{
					pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::ARRIVE);
					pActorSteerBehaviour->SetTarget(ESteerBehaviour::ARRIVE, gArrive2D.m_pTarget);
				}
			}
		}
	}

	if (gFlee2D.m_bEnable && !gFlee2D.m_bInitialized)
	{
		gFlee2D.m_bInitialized = true;

		gFlee2D.m_pTarget = Utils2D::create2DSwastikaShip();
		{
			gFlee2D.m_pTarget->SetPosition(Vector3(100, 100, 0));
			gFlee2D.m_pTarget->SetScale(Vector3(0.25, 0.25, 0.25));
		}

		gFlee2D.m_pSeeker = Utils2D::create2DSwastikaShip();
		{
			gFlee2D.m_pSeeker->SetPosition(Vector3(50, 50, 0));
			gFlee2D.m_pSeeker->SetScale(Vector3(0.25, 0.25, 0.25));
			gFlee2D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
			{
				ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gFlee2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
				if (pActorSteerBehaviour != nullptr)
				{
					pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::FLEE);
					pActorSteerBehaviour->SetTarget(ESteerBehaviour::FLEE, gFlee2D.m_pTarget);
				}
			}
		}
	}

	if (gWander2D.m_bEnable && !gWander2D.m_bInitialized)
	{
		gWander2D.m_bInitialized = true;

		gWander2D.m_pSeeker = Utils2D::create2DSwastikaShip();
		{
			gWander2D.m_pSeeker->SetPosition(Vector3(50, 50, 0));
			gWander2D.m_pSeeker->SetScale(Vector3(0.25, 0.25, 0.25));
			gWander2D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
			{
				ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gWander2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
				if (pActorSteerBehaviour != nullptr)
				{
					pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::WANDER);
				}
			}
		}
	}
}

void SteerBehaviourUtils::init3D()
{
	if (gSeek3D.m_bEnable && !gSeek3D.m_bInitialized)
	{
		gSeek3D.m_bInitialized = true;

		gSeek3D.m_pTarget = new Actor3D("../Content/cube.obj", "../Content/sp3bot.tga");
		gSeek3D.m_pTarget->SetScale(Vector3(0.1f, 0.1f, 0.2f));
		gSeek3D.m_pTarget->SetPosition(Vector3(2, 2, 2));
		gSeek3D.m_pTarget->SetCamera(m_pCamera);

		gSeek3D.m_pSeeker = new Actor3D("../Content/Arrow.obj", "../Content/sp3bot.tga");
		gSeek3D.m_pSeeker->SetScale(Vector3(0.1f, 0.1f, 0.2f));
		gSeek3D.m_pSeeker->SetPosition(Vector3(0, 0, 0));
		gSeek3D.m_pSeeker->SetCamera(m_pCamera);

		gSeek3D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
		{
			ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gSeek3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
			if (pActorSteerBehaviour != nullptr)
			{
				pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::SEEK);
				pActorSteerBehaviour->SetTarget(ESteerBehaviour::SEEK, gSeek3D.m_pTarget);
			}
		}

		gSeek3D.m_vTarget3DPositions.push_back(Vector3(2, 2, 2));
		gSeek3D.m_vTarget3DPositions.push_back(Vector3(2, -1, 0));
		gSeek3D.m_vTarget3DPositions.push_back(Vector3(-2, 2, 2));
		gSeek3D.m_vTarget3DPositions.push_back(Vector3(-2, -1, 0));

		gSeek3D.m_iSeekPos = 0;
	}
	if (gArrive3D.m_bEnable && !gArrive3D.m_bInitialized)
	{
		gArrive3D.m_bInitialized = true;

		gArrive3D.m_pTarget = new Actor3D("../Content/cube.obj", "../Content/sp3bot.tga");
		gArrive3D.m_pTarget->SetScale(Vector3(0.1f, 0.1f, 0.2f));
		gArrive3D.m_pTarget->SetPosition(Vector3(2, 2, 2));
		gArrive3D.m_pTarget->SetCamera(m_pCamera);

		gArrive3D.m_pSeeker = new Actor3D("../Content/Arrow.obj", "../Content/sp3bot.tga");
		gArrive3D.m_pSeeker->SetScale(Vector3(0.1f, 0.1f, 0.2f));
		gArrive3D.m_pSeeker->SetPosition(Vector3(0, 0, 0));
		gArrive3D.m_pSeeker->SetCamera(m_pCamera);

		gArrive3D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
		{
			ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gArrive3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
			if (pActorSteerBehaviour != nullptr)
			{
				pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::ARRIVE);
				pActorSteerBehaviour->SetTarget(ESteerBehaviour::ARRIVE, gArrive3D.m_pTarget);
			}
		}

		gArrive3D.m_vTarget3DPositions.push_back(Vector3(2, 2, 2));
		gArrive3D.m_vTarget3DPositions.push_back(Vector3(2, -1, 0));
		gArrive3D.m_vTarget3DPositions.push_back(Vector3(-2, 2, 2));
		gArrive3D.m_vTarget3DPositions.push_back(Vector3(-2, -1, 0));

		gArrive3D.m_iArrivePos = 0;
	}
	if (gFlee3D.m_bEnable && !gFlee3D.m_bInitialized)
	{
		gFlee3D.m_bInitialized = true;

		gFlee3D.m_pTarget = new Actor3D("../Content/cube.obj", "../Content/sp3bot.tga");
		gFlee3D.m_pTarget->SetScale(Vector3(0.1f, 0.1f, 0.1f));
		gFlee3D.m_pTarget->SetPosition(Vector3(0, 0, -0.1));
		gFlee3D.m_pTarget->SetCamera(m_pCamera);

		gFlee3D.m_pBox = new Actor3D("../Content/cube.obj", "../Content/sp3bot.tga");
		gFlee3D.m_pBox->SetScale(Vector3(6.0f, 6.0f, 6.0f));
		gFlee3D.m_pBox->SetPosition(Vector3(0, 0, 0));
		gFlee3D.m_pBox->SetCamera(m_pCamera);

		gFlee3D.m_pSeeker = new Actor3D("../Content/Arrow.obj", "../Content/sp3bot.tga");
		gFlee3D.m_pSeeker->SetScale(Vector3(0.1f, 0.1f, 0.2f));
		gFlee3D.m_pSeeker->SetPosition(Vector3(0, 0, 0));
		gFlee3D.m_pSeeker->SetCamera(m_pCamera);

		gFlee3D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
		{
			ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gFlee3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
			if (pActorSteerBehaviour != nullptr)
			{
				pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::FLEE);
				pActorSteerBehaviour->SetTarget(ESteerBehaviour::FLEE, gFlee3D.m_pTarget);
			}
		}
	}
	if (gWander3D.m_bEnable && !gWander3D.m_bInitialized)
	{
		gWander3D.m_bInitialized = true;

		gWander3D.m_pSeeker = new Actor3D("../Content/Arrow.obj", "../Content/sp3bot.tga");
		gWander3D.m_pSeeker->SetScale(Vector3(0.1f, 0.1f, 0.2f));
		gWander3D.m_pSeeker->SetPosition(Vector3(0, 0, 0));
		gWander3D.m_pSeeker->SetCamera(m_pCamera);

		gWander3D.m_pSeeker->AddBehaviour(ActorBehaviourType::STEER);
		{
			ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gWander3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
			if (pActorSteerBehaviour != nullptr)
			{
				pActorSteerBehaviour->SetBehaviour(ESteerBehaviour::WANDER);
				pActorSteerBehaviour->SetTarget(ESteerBehaviour::WANDER, nullptr);
			}
		}
	}
}

void SteerBehaviourUtils::render2D(int32_t iDeltaTimeMs)
{
	ActorSteerBehaviour* pActorSteerBehaviour = nullptr;
	if(gSeek2D.m_bEnable)
	{
		pActorSteerBehaviour = (ActorSteerBehaviour*)gSeek2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		if (pActorSteerBehaviour != nullptr)
		{
			Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

			Vector3 vLookAt = gSeek2D.m_pSeeker->GetPosition() + (vVelocity * 2);
			gSeek2D.m_pSeeker->LookAt(vLookAt);

			gSeek2D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
			gSeek2D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
		}
	}

	if(gArrive2D.m_bEnable)
	{
		pActorSteerBehaviour = (ActorSteerBehaviour*)gArrive2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		if (pActorSteerBehaviour != nullptr)
		{
			Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

			Vector3 vLookAt = gArrive2D.m_pSeeker->GetPosition() + (vVelocity * 2);
			gArrive2D.m_pSeeker->LookAt(vLookAt);

			gArrive2D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
			gArrive2D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
		}
	}

	if (gFlee2D.m_bEnable)
	{
		pActorSteerBehaviour = (ActorSteerBehaviour*)gFlee2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		if (pActorSteerBehaviour != nullptr)
		{
			Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

			Vector3 vLookAt = gFlee2D.m_pSeeker->GetPosition() + (vVelocity * 2);
			gFlee2D.m_pSeeker->LookAt(vLookAt);

			check2DBounds(gFlee2D.m_pSeeker);

			gFlee2D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
			gFlee2D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
		}
	}

	if (gWander2D.m_bEnable)
	{
		pActorSteerBehaviour = (ActorSteerBehaviour*)gWander2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		if (pActorSteerBehaviour != nullptr)
		{
			Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

			Vector3 vLookAt = gWander2D.m_pSeeker->GetPosition() + (vVelocity * 2);
			gWander2D.m_pSeeker->LookAt(vLookAt);

			check2DBounds(gWander2D.m_pSeeker);
			gWander2D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
		}
	}
}

void SteerBehaviourUtils::render3D(int32_t iDeltaTimeMs)
{
	float fDelatTimeMs = iDeltaTimeMs * 0.001f;

	if (gSeek3D.m_bEnable && gSeek3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER) != nullptr)
	{
		ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gSeek3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

		Vector3 vLookAt = gSeek3D.m_pSeeker->GetPosition() + (vVelocity * 1);
		const float EPSILON = 0.00000001f;
		if (vLookAt.length() > EPSILON)
		{
			gSeek3D.m_pSeeker->LookAt(vLookAt);
		}

		gSeek3D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
		gSeek3D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
	}
	if (gArrive3D.m_bEnable && gArrive3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER) != nullptr)
	{
		ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gArrive3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

		Vector3 vLookAt = gArrive3D.m_pSeeker->GetPosition() + (vVelocity * 2);
		const float EPSILON = 0.00000001f;
		if (vLookAt.length() > EPSILON)
		{
			gArrive3D.m_pSeeker->LookAt(vLookAt);
		}

		gArrive3D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
		gArrive3D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
	}
	if (gFlee3D.m_bEnable && gFlee3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER) != nullptr)
	{
		ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gFlee3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

		Vector3 vLookAt = gFlee3D.m_pSeeker->GetPosition() + (vVelocity * 1);
		const float EPSILON = 0.00000001f;
		if (vLookAt.length() > EPSILON)
		{
			gFlee3D.m_pSeeker->LookAt(vLookAt);
		}

		gFlee3D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
		{
			float fAngle = (m_iElapsedTimeMs * 0.0025f);

			float fX = 1.5 * sin(fAngle);
			float fY = 1.5 * cos(-fAngle);
			float fZ = 1.5 * sin(-fAngle);
			gFlee3D.m_pTarget->SetPosition(Vector3(fX, fY, fZ));
			gFlee3D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
		}
		//gFlee3D.m_pBox->Update(m_pGraphics, iDeltaTimeMs);

		check3DBounds(gFlee3D.m_pSeeker);
	}
	if (gWander3D.m_bEnable && gWander3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER) != nullptr)
	{
		ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gWander3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

		Vector3 vLookAt = gWander3D.m_pSeeker->GetPosition() + (vVelocity * 1);
		const float EPSILON = 0.00000001f;
		if (vLookAt.length() > EPSILON)
		{
			gWander3D.m_pSeeker->LookAt(vLookAt);
		}

		gWander3D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
		check3DBounds(gWander3D.m_pSeeker);
	}
}

void SteerBehaviourUtils::onMouseUpEx(int mCode, int x, int y)
{
	if (gSeek2D.m_bEnable)
	{
		gSeek2D.m_pTarget->SetPosition(Vector3(x, y, 0));
	}
	if (gSeek3D.m_bEnable)
	{
		gSeek3D.m_iSeekPos++;
		if (gSeek3D.m_iSeekPos >= gSeek3D.m_vTarget3DPositions.size())
			gSeek3D.m_iSeekPos = 0;

		gSeek3D.m_pTarget->SetPosition(gSeek3D.m_vTarget3DPositions.at(gSeek3D.m_iSeekPos));
	}
	if (gArrive2D.m_bEnable)
	{
		gArrive2D.m_pTarget->SetPosition(Vector3(x, y, 0));
	}
	if (gArrive3D.m_bEnable)
	{
		gArrive3D.m_iArrivePos++;
		if (gArrive3D.m_iArrivePos >= gArrive3D.m_vTarget3DPositions.size())
			gArrive3D.m_iArrivePos = 0;

		gArrive3D.m_pTarget->SetPosition(gArrive3D.m_vTarget3DPositions.at(gArrive3D.m_iArrivePos));
	}
	if (gFlee2D.m_bEnable)
	{
		gFlee2D.m_pTarget->SetPosition(Vector3(x, y, 0));
	}
}

void SteerBehaviourUtils::keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch)
{
	if (ch == 'T' || ch == 't')
	{
		m_b2DToggle = !m_b2DToggle;

		gSeek2D.m_bEnable = false;
		gSeek3D.m_bEnable = false;
		gArrive2D.m_bEnable = false;
		gArrive3D.m_bEnable = false;
		gFlee2D.m_bEnable = false;
		gFlee3D.m_bEnable = false;
		gWander2D.m_bEnable = m_b2DToggle;
		gWander3D.m_bEnable = !m_b2DToggle;

		initContent();
	}
}

void SteerBehaviourUtils::check2DBounds(Actor2D* pActor2D)
{
	if (pActor2D != nullptr)
	{
		EngineManager* pEngineManager = EngineManager::getInstance();

		bool bDirty = false;
		Vector3 vPosition = pActor2D->GetPosition();
		if (vPosition.x > pEngineManager->getWidth())
		{
			bDirty = true;
			vPosition.x = 0;
		}

		if (vPosition.x < 0)
		{
			bDirty = true;
			vPosition.x = pEngineManager->getWidth();
		}

		if (vPosition.y > pEngineManager->getHeight())
		{
			bDirty = true;
			vPosition.y = 0;
		}

		if (vPosition.y < 0)
		{
			bDirty = true;
			vPosition.y = pEngineManager->getHeight();
		}

		if(bDirty)
			pActor2D->SetPosition(vPosition);
	}
}

void SteerBehaviourUtils::check3DBounds(Actor3D* pActor3D)
{
	if (pActor3D != nullptr)
	{
		EngineManager* pEngineManager = EngineManager::getInstance();
		const float fBound = 3.0f;

		bool bDirty = false;
		Vector3 vPosition = pActor3D->GetPosition();
		if (vPosition.x > fBound)
		{
			bDirty = true;
			vPosition.x = -fBound;
		}
		else
		if (vPosition.x < -fBound)
		{
			bDirty = true;
			vPosition.x = fBound;
		}

		if (vPosition.y > fBound)
		{
			bDirty = true;
			vPosition.y = -fBound;
		}
		else
		if (vPosition.y < -fBound)
		{
			bDirty = true;
			vPosition.y = fBound;
		}

		if (vPosition.z > fBound)
		{
			bDirty = true;
			vPosition.z = -fBound;
		}
		else
		if (vPosition.z < -fBound)
		{
			bDirty = true;
			vPosition.z = fBound;
		}

		if (bDirty)
			pActor3D->SetPosition(vPosition);
	}
}