#pragma once
#include <vector>

namespace visions2D {
	
	class BoxCollider;

	struct CollisionInfo {
		glm::vec2 PointOfCollision;
		BoxCollider* CollidedWith;
		Entity* CollidedEntity;
	};

	class CollisionWorld {
	public:
		CollisionWorld();
		~CollisionWorld();

		static CollisionWorld* s_Instance;

		bool Initialize();
		void Render();
		void Shutdown();

		void VerifyAllCollisions();
		bool SegmentCast(glm::vec2 StartingPoint, glm::vec2 EndingPoint, CollisionInfo& OutCollisionInfo);
		bool Overlaps(BoxCollider* a, BoxCollider* b);

		void AddColliderToWorld(BoxCollider* Collider);
		void RemoveColliderFromWorld(BoxCollider* Collider);

	private:
		std::vector<BoxCollider*> m_WorldColliders;
	};
}