#pragma once
#include <glm/glm.hpp>
#include "Color.h"

namespace visions2D {
	class Texture;

	struct RenderData {
		Texture* Texture;
		glm::vec2 TextureScale;
		float* TexCoords;

		float WorldRotation;
		glm::vec2 WorldPosition;
		glm::vec2 WorldScale;

		// int DrawOrder;
		Color tint;

		// float Width;
		// float Height;
	};
}