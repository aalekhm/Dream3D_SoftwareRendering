#pragma once
#include <stdint.h>

enum ActorBehaviourType
{
	STEER
};

class RenderContext;
class ActorBehaviour
{
	public:
		virtual void													Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs) = 0;
	protected:
	private:
};