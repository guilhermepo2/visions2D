#pragma once
#include <glm/glm.hpp>

namespace visions2D {
	class LineSegment {
	public:
		LineSegment(const glm::vec2& Start, const glm::vec2& End);
		glm::vec2 PointOnSegment(float t) const;

	private:
		glm::vec2 m_Start;
		glm::vec2 m_End;
	};
}