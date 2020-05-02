#include "ActorSteerBehaviour.h"
#include "RenderContext.h"
#include "Actor.h"

#define SET_VAL(__val__, __is2D__) __val__ * (__is2D__ ? 15 : 1);

ActorSteerBehaviour::ActorSteerBehaviour()
{
	Init(nullptr);
}

ActorSteerBehaviour::ActorSteerBehaviour(Actor* pActor)
{
	Init(pActor);
}

void ActorSteerBehaviour::Init(Actor* pActor)
{
	if(pActor != nullptr)
	{
		bool bIs2D = pActor->Is2D();

		m_pActor = pActor;
		m_eBehaviour = 0;
		m_eSummingMethod = ESteerSummingMethod::WEIGHTED_AVERAGE;
		m_eDecleration = ESteerDeceleration::SLOW;
		m_fMaxSpeed = SET_VAL(15, bIs2D);
		m_fMaxForce = SET_VAL(40, bIs2D);
		m_fMass = 1;
		m_vVelocity = Vector3(0, 0, 0);
			
		for (int32_t i = 0; i < ESteerBehaviour::COUNT; i++)
		{
			m_fWeightedSum[i] = SET_VAL(20.0f, bIs2D);
			m_vTarget[i] = nullptr;
		}
	}
}

ActorSteerBehaviour::~ActorSteerBehaviour()
{
}

void ActorSteerBehaviour::Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs)
{
	float fDeltaMs = iDeltaTimeMs * 0.001f;

	Vector3 vSteerForce;

	// Calculate the combined force from each steering behavior in the Actor's list
	vSteerForce = Calculate();

	// Acceleration = Force/Mass
	Vector3 vAcceleration = vSteerForce / m_fMass;

	// Update velocity
	m_vVelocity += vAcceleration * fDeltaMs;

	// Make sure Actor does not exceed maximum speed
	m_vVelocity.truncate(m_fMaxSpeed);

	// Update the position
	m_pActor->SetPosition(m_pActor->GetPosition() + (m_vVelocity * fDeltaMs));
}

void ActorSteerBehaviour::LimitSpeed()
{
	m_vVelocity.truncate(m_fMaxSpeed);
}

Vector3 ActorSteerBehaviour::GetVelocity()
{
	return m_vVelocity;
}

void ActorSteerBehaviour::SetBehaviour(ESteerBehaviour eESteerBehaviour)
{
	m_eBehaviour = (1 << eESteerBehaviour);
}

void ActorSteerBehaviour::AddBehaviour(ESteerBehaviour eESteerBehaviour)
{
	m_eBehaviour |= (1 << eESteerBehaviour);
}

bool ActorSteerBehaviour::IsOn(ESteerBehaviour eESteerBehaviour)
{
	return (m_eBehaviour & (1 <<eESteerBehaviour)) > 0;
}

void ActorSteerBehaviour::SeekTarget(Actor* pTarget)
{
	m_vTarget[ESteerBehaviour::SEEK] = pTarget;
}

Vector3 ActorSteerBehaviour::Seek(Vector3 vTargetPos)
{
	Vector3 vDesiredVelocity = (vTargetPos - m_pActor->GetPosition());
	vDesiredVelocity.normalize();
	vDesiredVelocity *= m_fMaxSpeed;

	Vector3 vRetVector = (vDesiredVelocity - m_vVelocity);
	return vRetVector;
}

Vector3 ActorSteerBehaviour::Calculate()
{
	// Reset the Steering Force
	m_vForce.zero();

	switch (m_eSummingMethod)
	{
	case ESteerSummingMethod::WEIGHTED_AVERAGE:
	{
		m_vForce = CalculateWeightedSum();
	}
	break;
	case ESteerSummingMethod::PRIORITIZED:
	{
		m_vForce = CalculatePrioritized();
	}
	break;
	case ESteerSummingMethod::DITHERED:
	{
		m_vForce = CalculateDithered();
	}
	break;
	}

	return m_vForce;
}

Vector3 ActorSteerBehaviour::CalculateWeightedSum()
{
	Actor* pTarget = nullptr;

	if (IsOn(ESteerBehaviour::SEEK))
	{
		pTarget = m_vTarget[ESteerBehaviour::SEEK];
		Vector3 v3TargetPos = pTarget->GetPosition();

		if (pTarget != nullptr)
		{
			m_vForce += Seek(v3TargetPos) * m_fWeightedSum[ESteerBehaviour::SEEK];
			m_vForce.truncate(m_fMaxForce);
		}
	}

	return m_vForce;
}

Vector3 ActorSteerBehaviour::CalculatePrioritized()
{
	return Vector3(0, 0, 0);
}

Vector3 ActorSteerBehaviour::CalculateDithered()
{
	return Vector3(0, 0, 0);
}
