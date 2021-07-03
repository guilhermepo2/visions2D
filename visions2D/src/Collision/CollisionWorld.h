#pragma once
#include <vector>
#include <glm/glm.hpp>

namespace visions2D {
	
	class BoxCollider;
	class Entity;
	class Renderer;

	struct CollisionInfo {
		glm::vec2 PointOfCollision;
		BoxCollider* CollidedWith;
		Entity* CollidedEntity;
	};

	// TODO: Have a better name for this
	struct CollisionHappened {
	public:
		BoxCollider* a;
		BoxCollider* b;

		bool operator==(const CollisionHappened& other) {
			return (this->a == other.a && this->b == other.b);
		}
	};

	class CollisionWorld {
	public:
		CollisionWorld();
		~CollisionWorld();

		static CollisionWorld* s_Instance;

		bool Initialize();
		void Render(Renderer* RendererReference);
		void Shutdown();

		void VerifyAllCollisions();
		bool SegmentCast(glm::vec2 StartingPoint, glm::vec2 EndingPoint, CollisionInfo& OutCollisionInfo);
		bool Overlaps(BoxCollider* a, BoxCollider* b);
		bool HasCollisionAt(float x, float y);
		bool HasCollisionAt(glm::vec2 Position);

		void AddColliderToWorld(BoxCollider* Collider);
		void RemoveColliderFromWorld(BoxCollider* Collider);

	private:
		std::vector<BoxCollider*> m_WorldColliders;
		std::vector<CollisionHappened> m_LastFrameCollisions;
	};
}