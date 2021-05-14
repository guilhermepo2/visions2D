#include "Entity.h"
#include "Component/Component.h"

namespace visions2D {
	Entity::Entity(const std::string& InName) : Name(InName) {}

	void Entity::BeginPlay() {
		for (Component* c : m_Components) {
			c->BeginPlay();
		}
	}

	bool Entity::ProcessInput(const InputState& CurrentInputState) {
		for (Component* c : m_Components) {
			if (c->ProcessInput(CurrentInputState)) {
				return true;
			}
		}

		return false;
	}

	void Entity::Update(float DeltaTime) {
		for (Component* c : m_Components) {
			c->Update(DeltaTime);
		}
	}

	void Entity::Render() {

		// TODO
		// Should this really be here?!
		// lol, no?!
		/*
		if (HasComponentOfType<Transform>() && HasComponentOfType<Sprite>()) {
			Transform* t = GetComponentOfType<Transform>();
			Sprite* s = GetComponentOfType<Sprite>();

			Renderer::s_Instance->AddToRenderQueue(s->tex, &(t->Position), t->Rotation, &(t->Scale), s->DrawOrder);
		}
		*/

		for (Component* c : m_Components) {
			c->Render();
		}
	}

	void Entity::Destroy() {
		for (Component* c : m_Components) {
			c->Destroy();
		}

		// TODO: Properly destroy and mark for destruction ?!
		this->m_bIsActive = false;
	}
}