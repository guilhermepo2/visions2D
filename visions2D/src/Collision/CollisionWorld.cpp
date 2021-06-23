#include "CollisionWorld.h"
#include "Log.h"
#include "Entity/Component/BoxCollider.h"
#include "Collider.h"
#include "Renderer/Renderer.h"

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

	void CollisionWorld::Render(Renderer* RendererReference) {
		for (BoxCollider* collider : m_WorldColliders) {
			RenderData rd;
			TransformComponent* t = collider->Owner->GetComponentOfType<TransformComponent>();
			BoxCollider* b = collider->Owner->GetComponentOfType<BoxCollider>();
			Math::Rectangle rect = b->GetWorldPositionRectangle();

			rd.Texture = nullptr;
			rd.TextureScale = glm::vec2(rect.Width(), rect.Height());
			rd.TexCoords = nullptr;
			rd.WorldRotation = 0;
			rd.WorldPosition = glm::vec2(rect.Position().x, rect.Position().y);
			rd.WorldScale = t->Scale;
			rd.tint = visions2D::Color(1.0f, 0.0f, 0.0f, 0.5f);

			// LOG_INFO("Collision Rendering on Position ({0},{1}) and Texture Scale ({2},{3})", rect.Position().x, rect.Position().y, rect.Width(), rect.Height());

			RendererReference->SpriteRenderData.push_back(rd);
		}
	}

	// TODO: Clean all BoxColliders?
	void CollisionWorld::Shutdown() { } // TODO 

	void CollisionWorld::VerifyAllCollisions() {
		std::vector<CollisionHappened> CollisionsThisFrame;

		for (size_t i = 0; i < m_WorldColliders.size(); i++) {
			for (size_t j = i + 1; j < m_WorldColliders.size(); j++) {
				BoxCollider* a = m_WorldColliders[i];
				BoxCollider* b = m_WorldColliders[j];

				if (Overlaps(a, b)) {
					CollisionsThisFrame.push_back({ a, b });
				}
			}
		}

		// Checking collisions that happened against last frame collisions
		for (int i = 0; i < CollisionsThisFrame.size(); i++) {
			// First, dispatch all new collisions anyway to "Handle Collision Callback"
			CollisionsThisFrame[i].a->HandleCollisionCallback(CollisionsThisFrame[i].b);
			CollisionsThisFrame[i].b->HandleCollisionCallback(CollisionsThisFrame[i].a);

			auto IsCollisionRepeated = std::find(m_LastFrameCollisions.begin(), m_LastFrameCollisions.end(), CollisionsThisFrame[i]);
			if (IsCollisionRepeated == m_LastFrameCollisions.end()) { // this means we didn't find it
				// dispatching on collision enter
				CollisionsThisFrame[i].a->HandleOnCollisionEnter(CollisionsThisFrame[i].b);
				CollisionsThisFrame[i].b->HandleOnCollisionEnter(CollisionsThisFrame[i].a);
			}
			else {
				// we found it, so dispatching on collision stay
				CollisionsThisFrame[i].a->HandleOnCollisionStay(CollisionsThisFrame[i].b);
				CollisionsThisFrame[i].b->HandleOnCollisionStay(CollisionsThisFrame[i].a);
			}
		}

		// Checking which collisions no longer happen
		for (int i = 0; i < m_LastFrameCollisions.size(); i++) {
			auto CollisionNoLongerHappens = std::find(CollisionsThisFrame.begin(), CollisionsThisFrame.end(), m_LastFrameCollisions[i]);

			if (CollisionNoLongerHappens == CollisionsThisFrame.end()) {
				// we didn't find the collision, this means it no longers happen
				m_LastFrameCollisions[i].a->HandleOnCollisionExit(m_LastFrameCollisions[i].b);
				m_LastFrameCollisions[i].b->HandleOnCollisionExit(m_LastFrameCollisions[i].a);
			}
		}

		m_LastFrameCollisions.clear();

		// TODO: I'm not sure what happens here... is it a copy assignment? rerence? what?
		m_LastFrameCollisions = CollisionsThisFrame;
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