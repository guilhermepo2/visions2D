#include "GameWorld.h"
#include "Entity.h"

namespace visions2D {
	void GameWorld::BeginPlay() {
		for (Entity* e : m_Entities) {
			e->BeginPlay();
		}
	}
	
	bool GameWorld::ProcessInput(const InputState& CurrentInputState) {

		// The behavior of this is that only one entity will consume the input and then this function will end... (if return true)
		// I guess it kind of make sense?
		for (Entity* e : m_Entities) {
			if (e->ProcessInput(CurrentInputState)) {
				return true;
			}
		}

		return false;
	}
	
	void GameWorld::Update(float DeltaTime) {
		for (Entity* e : m_Entities) {
			e->Update(DeltaTime);
		}
	}
	
	void GameWorld::Render(Renderer* RendererReference) {
		for (Entity* e : m_Entities) {
			e->Render(RendererReference);
		}
	}
	
	void GameWorld::Destroy() {
		for (Entity* e : m_Entities) {
			e->Destroy();
		}

		for (int i = 0; i < m_Entities.size(); i++) {
			delete m_Entities[i];
		}

		m_Entities.clear();
	}

	Entity* GameWorld::AddEntity(const std::string& EntityName) {
		Entity* NewEntity = new Entity(EntityName);
		m_Entities.push_back(NewEntity);
		return NewEntity;
	}

	Entity* GameWorld::GetEntityByName(const std::string& EntityName) {
		for (int i = 0; i < m_Entities.size(); i++) {
			if (m_Entities[i]->Name == EntityName) {
				return m_Entities[i];
			}
		}

		return nullptr;
	}
}