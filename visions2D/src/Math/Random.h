#pragma once
#include <glm/gtc/random.hpp>

namespace visions2D {
	namespace Random {
		static void Initialize() {
			srand(time(nullptr));
		}

		static float AbsoluteValue() {
			return glm::linearRand(0.0f, 1.0f);
		}

		static float Value() {
			return glm::linearRand(-1.0f, 1.0f);
		}

		static float Range(float min, float max) {
			return glm::linearRand(min, max);
		}
	}
}