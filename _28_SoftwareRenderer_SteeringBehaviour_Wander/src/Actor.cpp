#include "Actor.h"
#include "Sprite.h"
#include "ActorSteerBehaviour.h"

Actor::Actor(std::string sSpriteName)
: m_pBitmapTexture(nullptr)
, m_bVisible(true)
, m_vPos(0, 0, 0)
, m_vScale(1, 1, 1)
, m_vHeading(0, 0, 0)
, m_bIs2D(false)
{
	if (!sSpriteName.empty())
	{
		m_pBitmapTexture = std::make_unique<Sprite>(sSpriteName.c_str());
	}
}

void Actor::Update(RenderContext* pRenderContext, int32_t iDeltaTimeMs)
{
	for (auto& pActorBehaviourItr : m_BehaviourMap)
	{
		ActorBehaviourType eActorBehaviourType = pActorBehaviourItr.first;
		ActorBehaviour* pActorBehaviour = pActorBehaviourItr.second.get();
		if (pActorBehaviour != nullptr)
		{
			pActorBehaviour->Update(pRenderContext, iDeltaTimeMs);
		}
	}
}

void Actor::AddBehaviour(ActorBehaviourType eActorBehaviourType)
{
	switch (eActorBehaviourType)
	{
		case ActorBehaviourType::STEER:
		{
			m_BehaviourMap[eActorBehaviourType] = std::move(std::make_unique<ActorSteerBehaviour>(this));
		}
		break;
	}
}

ActorBehaviour* Actor::GetBehaviour(ActorBehaviourType eActorBehaviourType)
{
	auto& pItr = m_BehaviourMap.find(eActorBehaviourType);
	if (pItr != m_BehaviourMap.end())
	{
		return pItr->second.get();
	}

	return nullptr;
}

Vector3 Actor::GetPosition()
{
	return m_vPos;
}

void Actor::SetPosition(Vector3 vPos)
{
	m_vPos = vPos;
	OnSetPosition();
}

void Actor::OnSetPosition() {}

Vector3 Actor::GetScale()
{
	return m_vScale;
}

void Actor::SetScale(Vector3 vScale)
{
	m_vScale = vScale;
	OnSetScale();
}

void Actor::OnSetScale() {}

bool Actor::IsVisible()
{
	return m_bVisible;
}

void Actor::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

void Actor::LookAt(Vector3 vLookAtPoint)
{
	m_vHeading = (m_vPos - vLookAtPoint);
	OnLookAt(vLookAtPoint);
}

void Actor::OnLookAt(Vector3 vLookAtPoint) {}

Sprite* Actor::GetTexture()
{
	return m_pBitmapTexture.get();
}

bool Actor::Is2D()
{
	return m_bIs2D;
}

Actor::~Actor()
{

}