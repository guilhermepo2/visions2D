#pragma once
#include "Entity/Entity.h"
#include "Component.h"
#include "TransformComponent.h"

namespace visions2D {

	class Tilesheet;

	class TileComponent : public Component {
	public:
		Tilesheet* tilesheet;
		int Data;
		int DrawOrder;

		TileComponent() : tilesheet(nullptr) {}
		TileComponent(Tilesheet* _Tilesheet, int _Data, int _DrawOrder) : tilesheet(_Tilesheet), Data(_Data), DrawOrder(_DrawOrder) {}

		void Render() override {
			// TODO
			// TransformComponent* transform = Owner->GetComponentOfType<TransformComponent>();
		}

	};
}
