#include "MeshCollider.hpp"

namespace game
{
	MeshCollider::MeshCollider()
	{
	}

	bool MeshCollider::Contains(Collider & collider)
	{
		size_t index = 0;
		Vector2 position;
		while (SetPositionByIndex(index, position))
		{
			++index;
			if (collider.Contains(position))
				return true;
#ifdef _DEBUG
			if (index > 10000)
				// PS: While SetPositionByIndex return false, loop ends
				throw std::overflow_error("One collider has more than ten thousand collision points. Have you forgotten to set the end conditions?");
#endif
		}
		return false;
	}

}