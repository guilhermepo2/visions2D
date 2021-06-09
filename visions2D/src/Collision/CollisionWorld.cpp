#include "CollisionWorld.h"
#include "Log.h"
#include "Entity/Component/BoxCollider.h"

namespace visions2D {
	CollisionWorld* CollisionWorld::s_Instance = nullptr;

	CollisionWorld::CollisionWorld() {}
	CollisionWorld::~CollisionWorld() {}

	bool CollisionWorld::Initialize() {
		if (s_Instance != nullptr) {
			LOG_ERROR("[collision world] are you trying to create two collision worlds?");
			return false;
		}

		s_Instance = this;
		return true;
	}

	void CollisionWorld::Render() {}

	// TODO: Clean all BoxColliders?
	void CollisionWorld::Shutdown() { } // TODO 

	void CollisionWorld::VerifyAllCollisions() {
		// TODO
	}

	void CollisionWorld::AddColliderToWorld(BoxCollider* Collider) {
		m_WorldColliders.push_back(Collider);
	}

	void CollisionWorld::RemoveColliderFromWorld(BoxCollider* Collider) {
		std::vector<BoxCollider*>::iterator BoxColliderIterator = std::find(m_WorldColliders.begin(), m_WorldColliders.end(), Collider);

		if (BoxColliderIterator != m_WorldColliders.end()) {
			std::iter_swap(BoxColliderIterator, m_WorldColliders.end() - 1);
			m_WorldColliders.pop_back();
		}
	}
}