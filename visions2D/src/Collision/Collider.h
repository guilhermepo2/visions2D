#pragma once
#include "LineSegment.h"
#include "Math/Rectangle.h"

namespace visions2D {
	static bool Collider_CheckCollision(const Math::Rectangle RectA, const Math::Rectangle RectB) {

		bool NotIntersected =
			RectA.MaxPoint().x < RectB.MinPoint().x ||
			RectA.MaxPoint().y < RectB.MinPoint().y ||
			RectB.MaxPoint().x < RectA.MinPoint().x ||
			RectB.MaxPoint().y < RectA.MinPoint().y;

		return !NotIntersected;
	}

	static bool Collider_CheckLineCollision(const LineSegment& l, const Math::Rectangle& Rect, float& OutT) {
		float steps = 0.1f;

		for (float i = 0.0f; i <= 1.0f; i += steps) {
			if (Rect.Contains(l.PointOnSegment(i))) {
				OutT = i;
				return true;
			}
		}

		return false;
	}
}