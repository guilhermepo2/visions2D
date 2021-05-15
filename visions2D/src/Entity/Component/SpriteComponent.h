#pragma once
#include "Component.h"

namespace visions2D {
	class Texture;

	class SpriteComponent : public Component {
	public:
		Texture* tex;
		int DrawOrder;

		SpriteComponent() : tex(nullptr) {}
		SpriteComponent(Texture* _Texture, int _DrawOrder) : tex(_Texture), DrawOrder(_DrawOrder) {}

		void Render() override {
			// TODO
			// TransformComponent* transform = Owner->GetComponentOfType<TransformComponent>();
		}
	};
}