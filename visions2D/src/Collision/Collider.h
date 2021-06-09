#pragma once
#include "Math/Rectangle.h"

namespace visions2D {
	static bool CheckCollision(const Math::Rectangle RectA, const Math::Rectangle RectB) {
		bool NotIntersected =
			RectA.MaxPoint().x < RectB.MinPoint().x ||
			RectA.MaxPoint().y < RectB.MinPoint().y ||
			RectB.MaxPoint().x < RectA.MinPoint().x ||
			RectB.MaxPoint().y < RectA.MinPoint().y;

		return !NotIntersected;
	}

	// TODO: Check Line Collision
}