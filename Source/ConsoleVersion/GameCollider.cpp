#include "GameCollider.hpp"
#include "vytAlgorithm.hpp"
#include <vector>

namespace game
{
	static std::vector<Collider*> activeColliders;

	Collider::Collider(bool isActive) : m_isActive(!isActive)
	{
		setColliderActive(isActive);
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

	void Collider::SimulateMove()
	{
		size_t index = 0;
		Vector2 position;
		while (SetPositionByIndex(index, position))
		{
			++index;
			for (auto&collider : activeColliders)
				if (collider != this && collider->Contains(position))
				{
					OnCollision(*collider);
					collider->OnCollision(*this);
				}
#ifdef _DEBUG
			if (index > 100)
				// PS: While SetPositionByIndex return false, loop ends
				throw std::overflow_error("One collider has more than one hundred collision points. Have you forgotten to set the end conditions?");
#endif
		}
	}

}