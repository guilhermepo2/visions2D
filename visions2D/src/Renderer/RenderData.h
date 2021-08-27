#pragma once
#include <glm/glm.hpp>
#include "Color.h"

namespace visions2D {
	class Texture;

	struct RenderData {
		Texture* Texture = nullptr;
		glm::vec2 TextureScale = { 1.0f, 1.0f };

		// TODO: Change this to "Source Rectangle"?
		float* TexCoords = nullptr;

		float WorldRotation = 0.0f;
		glm::vec2 WorldPosition = { 0.0f, 0.0f };
		glm::vec2 WorldScale{ 1.0f, 1.0f };

		// int DrawOrder;
		Color tint;

		// float Width;
		// float Height;
	};
}