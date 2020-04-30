#pragma once
#include "ActorBehaviour.h"
#include "Vector4f.h"

enum ESteerBehaviour
{
	SEEK	= 1,
	ARRIVE	= (SEEK << 1),
	FLEE	= (ARRIVE << 1),
	PURSUIT = (FLEE << 1),
	EVADE	= (PURSUIT << 1),
	WANDER	= (EVADE << 1),
	COUNT	= 7
};

enum ESteerSummingMethod
{
	WEIGHTED_AVERAGE,
	PRIORITIZED,
	DITHERED
};

enum ESteerDeceleration
{
	SLOW,
	NORMAL,
	FAST
};

class RenderContext;
class Actor;
class ActorSteerBehaviour : public ActorBehaviour
{
	public:
									ActorSteerBehaviour();
									ActorSteerBehaviour(Actor* pActor);
		virtual						~ActorSteerBehaviour();

		virtual void				Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs);

		Vector3						GetVelocity();

		void						SetBehaviour(ESteerBehaviour eESteerBehaviour);
		void						AddBehaviour(ESteerBehaviour eESteerBehaviour);

		void						SetTarget(ESteerBehaviour eESteerBehaviour, Actor* pTarget);

		void						LimitSpeed();
	protected:
		bool						IsOn(ESteerBehaviour eESteerBehaviour);

		Vector3						Seek(Vector3 vTargetPos);	// Moves the Agent towards a Target's Position
		Vector3						Arrive(Vector3 vTargetPos);	// Moves the Agent towards a Target's Position

		Vector3						Calculate();
		Vector3						CalculateWeightedSum();
		Vector3						CalculatePrioritized();
		Vector3						CalculateDithered();

		Vector3						m_vVelocity;
	private:
		void						Init(Actor* pActor);

		// Steering Behaviour
		int32_t						m_eBehaviour;
		Vector3						m_vForce;
		Vector3						m_vTargetPosition;
		double						m_fMaxSpeed;
		double						m_fMaxForce;
		double						m_fMass;
		double						m_fWeightedSum[ESteerBehaviour::COUNT];
		ESteerSummingMethod			m_eSummingMethod;
		ESteerDeceleration			m_eDecleration;
		Actor*						m_vTarget[ESteerBehaviour::COUNT];
		Actor*						m_pActor;
};
