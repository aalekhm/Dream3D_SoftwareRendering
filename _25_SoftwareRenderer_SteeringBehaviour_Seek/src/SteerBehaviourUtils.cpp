#include "SteerBehaviourUtils.h"
#include "Utils2D.h"
#include "ActorSteerBehaviour.h"
#include "RenderContext.h"
#include "Camera.h"
#include "Actor3D.h"

RenderContext*	SteerBehaviourUtils::m_pGraphics;
Camera*			SteerBehaviourUtils::m_pCamera;
bool			SteerBehaviourUtils::m_b2DToggle = true;

Seek2D		gSeek2D;
Seek3D		gSeek3D;

void SteerBehaviourUtils::init(RenderContext* pGraphics, Camera* pCamera)
{
	m_pGraphics = pGraphics;
	m_pCamera = pCamera;

	{
		gSeek2D.m_bEnable = false;
		gSeek3D.m_bEnable = true;
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
					pActorSteerBehaviour->SeekTarget(gSeek2D.m_pTarget);
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
				pActorSteerBehaviour->SeekTarget(gSeek3D.m_pTarget);
			}
		}

		gSeek3D.m_vTarget3DPositions.push_back(Vector3(2, 2, 2));
		gSeek3D.m_vTarget3DPositions.push_back(Vector3(2, -1, 0));
		gSeek3D.m_vTarget3DPositions.push_back(Vector3(-2, 2, 2));
		gSeek3D.m_vTarget3DPositions.push_back(Vector3(-2, -1, 0));

		gSeek3D.m_iSeekPos = 0;
	}
}

void SteerBehaviourUtils::render2D(int32_t iDeltaTimeMs)
{
	if(gSeek2D.m_bEnable)
	{
		if (gSeek2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER) != nullptr)
		{
			ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gSeek2D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
			Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

			Vector3 vLookAt = gSeek2D.m_pSeeker->GetPosition() + (vVelocity * 2);
			gSeek2D.m_pSeeker->LookAt(vLookAt);

			gSeek2D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
			gSeek2D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
		}
	}
}

void SteerBehaviourUtils::render3D(int32_t iDeltaTimeMs)
{
	if (gSeek3D.m_bEnable && gSeek3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER) != nullptr)
	{
		ActorSteerBehaviour* pActorSteerBehaviour = (ActorSteerBehaviour*)gSeek3D.m_pSeeker->GetBehaviour(ActorBehaviourType::STEER);
		Vector3 vVelocity = pActorSteerBehaviour->GetVelocity();

		Vector3 vLookAt = gSeek3D.m_pSeeker->GetPosition() + (vVelocity * 2);
		const float EPSILON = 0.00000001f;
		if (vLookAt.length() > EPSILON)
		{
			gSeek3D.m_pSeeker->LookAt(vLookAt);
		}

		gSeek3D.m_pSeeker->Update(m_pGraphics, iDeltaTimeMs);
		gSeek3D.m_pTarget->Update(m_pGraphics, iDeltaTimeMs);
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
}

void SteerBehaviourUtils::keyReleasedEx(unsigned int iVirtualKeycode, unsigned short ch)
{
	if (ch == 'T' || ch == 't')
	{
		m_b2DToggle = !m_b2DToggle;

		gSeek2D.m_bEnable = m_b2DToggle;
		gSeek3D.m_bEnable = !m_b2DToggle;

		initContent();
	}
}