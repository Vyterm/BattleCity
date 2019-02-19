#include "BoxCollider.hpp"

bool game::BoxCollider::Contains(Collider & collider)
{
	BoxCollider *pBox = dynamic_cast<BoxCollider*>(&collider);
	if (nullptr != pBox)
	{
		return
			getPosition().x + m_rect.right >=	pBox->getPosition().x + pBox->m_rect.left &&
			getPosition().x + m_rect.left <=	pBox->getPosition().x + pBox->m_rect.right &&
			getPosition().y + m_rect.bottom >=	pBox->getPosition().y + pBox->m_rect.top &&
			getPosition().y + m_rect.top <=		pBox->getPosition().y + pBox->m_rect.bottom;
	}
	else
	{
		Vector2 position(m_rect.left, m_rect.top);
		for (; position.y < m_rect.bottom; ++position.y)
			for (; position.x < m_rect.right; ++position.x)
				if (collider.Contains(position + getPosition()))
					return true;
	}
	return false;
}

bool game::BoxCollider::Contains(const Vector2 & position)
{
	return
		position.x >=	getPosition().x + m_rect.left &&
		position.x <	getPosition().x + m_rect.right &&
		position.y >=	getPosition().y + m_rect.top &&
		position.y <	getPosition().y + m_rect.bottom;
}
