#pragma once
#include "Component.h"
#include <glm/glm.hpp>
namespace visions2D {
	class TransformComponent : public Component {
	public:
		glm::vec2 Position;
		float Rotation;
		glm::vec2 Scale;

		TransformComponent() {}

		TransformComponent(const glm::vec2& InPosition, const float& InRotation, const glm::vec2& InScale) {
			Position = InPosition;
			Rotation = InRotation;
			Scale = InScale;
		}

		void Rotate(float DeltaRotation) {
			Rotation += DeltaRotation;
		}

		void Translate(glm::vec2 Delta) {
			Position += Delta;
		}

		void SetScaleXSign(int Sign) {
			Scale.x = static_cast<float>(Sign);
		}
	};
}