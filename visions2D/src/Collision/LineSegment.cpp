#include "LineSegment.h"

namespace visions2D {
	LineSegment::LineSegment(const glm::vec2& Start, const glm::vec2& End) : m_Start(Start), m_End(End) {}

	glm::vec2 LineSegment::PointOnSegment(float t) const {
		t = glm::clamp(t, 0.0f, 1.0f);
		return m_Start + ((m_End - m_Start) * t);
	}
}