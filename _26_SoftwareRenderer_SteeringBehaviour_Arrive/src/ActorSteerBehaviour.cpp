#include "ActorSteerBehaviour.h"
#include "RenderContext.h"
#include "Actor.h"

#define SET_VAL(__val2D__, __val3D__, __is2D__)  (__is2D__ ? __val2D__ : __val3D__);

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
		m_fMaxSpeed = SET_VAL(150, 1.0f, bIs2D);
		m_fMaxForce = SET_VAL(400, 1.0f, bIs2D);
		m_fMass = 1;
		m_vVelocity = Vector3(0, 0, 0);
			
		for (int32_t i = 0; i < ESteerBehaviour::COUNT; i++)
		{
			m_fWeightedSum[i] = SET_VAL(300, 1.0f, bIs2D);
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

void ActorSteerBehaviour::SetTarget(ESteerBehaviour eESteerBehaviour, Actor* pTarget)
{
	m_vTarget[eESteerBehaviour] = pTarget;
}

// This behavior moves the 'Seeker' towards its 'Target's position
Vector3 ActorSteerBehaviour::Seek(Vector3 vTargetPos)
{
	Vector3 vDesiredVelocity = (vTargetPos - m_pActor->GetPosition());	// Calculate the vector between Seeker & Target
	vDesiredVelocity.normalize();										// Normalize it, so that its value is between 0.0f - 1.0f
	vDesiredVelocity *= m_fMaxSpeed;									// Amplify it to the Max Speed the Seeker can attain.

	Vector3 vRetVector = (vDesiredVelocity - m_vVelocity);				// Calculate the difference between Seeker's Desired & Current velocity.
	return vRetVector;													// Return the difference that is the force that is needed to Steer the Seeker towards its Target.
}

// This behavior is similar to 'Seek' but it attempts to 'Arrive' at the 'Target's position with a Zero velocity.
Vector3 ActorSteerBehaviour::Arrive(Vector3 vTargetPos)
{
	Vector3 vDesiredVelocity = (vTargetPos - m_pActor->GetPosition());	// Calculate the vector between Seeker & Target
	vDesiredVelocity.truncate(m_fMaxSpeed);								// Truncate it such that it doesn't increase beyond the Seeker's Max Speed.

	Vector3 vRetVector = (vDesiredVelocity - m_vVelocity);				// Calculate the difference between Seeker's Desired & Current velocity.
	return vRetVector;													// Return the difference that is the force that is needed to Steer the Seeker towards its Target.
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

	if (IsOn(ESteerBehaviour::ARRIVE))
	{
		pTarget = m_vTarget[ESteerBehaviour::ARRIVE];
		Vector3 v3TargetPos = pTarget->GetPosition();

		if (pTarget != nullptr)
		{
			m_vForce += Arrive(v3TargetPos) * m_fWeightedSum[ESteerBehaviour::ARRIVE];
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
