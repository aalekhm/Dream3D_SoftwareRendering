#pragma once
#include "Vector4f.h"
#include <memory>
#include "ActorBehaviour.h"
#include <map>

class Sprite;
class Actor
{
	public:
																		Actor(std::string sSpriteName);
		virtual															~Actor();

		virtual void													Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs);
		void															AddBehaviour(ActorBehaviourType eActorBehaviourType);
		ActorBehaviour*													GetBehaviour(ActorBehaviourType eActorBehaviourType);

		Vector3															GetPosition();
		void															SetPosition(Vector3 vPos);

		Vector3															GetScale();
		void															SetScale(Vector3 vScale);

		bool															IsVisible();
		void															SetVisible(bool bVisible);

		void															LookAt(Vector3 vLookAtPoint);

		Sprite*															GetTexture();

		bool															Is2D();
	protected:
		virtual void													OnSetPosition();
		virtual void													OnSetScale();
		virtual void													OnLookAt(Vector3 vLookAtPoint);

		Vector3															m_vPos;
		Vector3															m_vScale;
		Vector3															m_vHeading;

		bool															m_bIs2D;
	private:
		bool															m_bVisible;

		std::unique_ptr<Sprite>											m_pBitmapTexture;

		std::map<ActorBehaviourType, std::unique_ptr<ActorBehaviour>>	m_BehaviourMap;
};