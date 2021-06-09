#pragma once
#include <vector>

namespace visions2D {
	
	class BoxCollider;

	class CollisionWorld {
	public:
		CollisionWorld();
		~CollisionWorld();

		static CollisionWorld* s_Instance;

		bool Initialize();
		void Render();
		void Shutdown();

		void VerifyAllCollisions();
		// TODO: SegmentCast

		void AddColliderToWorld(BoxCollider* Collider);
		void RemoveColliderFromWorld(BoxCollider* Collider);

	private:
		std::vector<BoxCollider*> m_WorldColliders;
	};
}