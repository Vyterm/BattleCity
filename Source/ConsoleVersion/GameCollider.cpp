#include "GameCollider.hpp"
#include "vytAlgorithm.hpp"
#include <vector>

namespace game
{
	static std::vector<Collider*> activeColliders;

	Collider::Collider() : m_isActive(false)
	{
	}

	void Collider::setColliderActive(bool isActive)
	{
		if (isActive == m_isActive) return;
		m_isActive = isActive;
		if (m_isActive)
			activeColliders.push_back(this);
		else
		{
			vyt::Remove(activeColliders, this);
		}
	}

	Collider::~Collider()
	{
		setColliderActive(false);
	}

	void Collider::StrikeToActiveColliders()
	{
		for (auto&collider : activeColliders)
			if (collider != this && this->Contains(*collider))
			{
				OnCollision(*collider);
				collider->OnCollision(*this);
			}
	}

}