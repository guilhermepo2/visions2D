#include "CollisionWorld.h"
#include "Log.h"
#include "Entity/Component/BoxCollider.h"
#include "Collider.h"

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
		for (size_t i = 0; i < m_WorldColliders.size(); i++) {
			for (size_t j = i + 1; j < m_WorldColliders.size(); j++) {
				BoxCollider* a = m_WorldColliders[i];
				BoxCollider* b = m_WorldColliders[j];

				if (Overlaps(a, b)) {
					a->HandleCollisionCallback(b);
					b->HandleCollisionCallback(a);
				}
			}
		}
	}

	bool CollisionWorld::Overlaps(BoxCollider* a, BoxCollider* b) {
		return Collider_CheckCollision(
			a->GetWorldPositionRectangle(),
			b->GetWorldPositionRectangle()
		);
	}

	bool CollisionWorld::SegmentCast(glm::vec2 StartingPoint, glm::vec2 EndingPoint, CollisionInfo& OutCollisionInfo) {
		bool bCollided = false;
		LineSegment l(StartingPoint, EndingPoint);
		float closestT = 1.1f; // t will be maximum 1.0f, so 1.1f is enough?

		for (BoxCollider* collider : m_WorldColliders) {
			float t;
			Math::Rectangle colliderRect = collider->GetWorldPositionRectangle();

			if (Collider_CheckLineCollision(l, colliderRect, t)) {
				if (t < closestT) {
					OutCollisionInfo.PointOfCollision = l.PointOnSegment(t);
					OutCollisionInfo.CollidedWith = collider;
					OutCollisionInfo.CollidedEntity = collider->Owner;
					bCollided = true;
				}
			}
		}

		return bCollided;
	}

	// TODO: Return what has collided with?!
	bool CollisionWorld::HasCollisionAt(float x, float y) {
		for (int i = 0; i < m_WorldColliders.size(); i++) {
			if (m_WorldColliders[i]->GetWorldPositionRectangle().Contains(x, y)) {
				return true;
			}
		}

		return false;
	}

	bool CollisionWorld::HasCollisionAt(glm::vec2 Position) {
		return HasCollisionAt(Position.x, Position.y);
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