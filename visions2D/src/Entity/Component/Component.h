#pragma once
#include "Common.h"

namespace visions2D {
	class Entity;
	struct InputState;

	class Component {
	public:
		Entity* Owner;

		Component() {}
		virtual ~Component() {}
		virtual void Initialize() {}
		virtual void BeginPlay() {}
		virtual bool ProcessInput(const InputState& CurrentInputState) { unreferenced(CurrentInputState); return false; }
		virtual void Update(float DeltaTime) { unreferenced(DeltaTime); }
		virtual void Render() {}
		virtual void Destroy() {}
	};
}