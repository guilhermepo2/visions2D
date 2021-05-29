#pragma once
#include <vector>
#include <string>

namespace visions2D {

	class Entity;
	struct InputState;

	class GameWorld {
	public:
		void BeginPlay();
		bool ProcessInput(const InputState& CurrentInputState);
		void Update(float DeltaTime);
		void Render();
		void Destroy();

		inline size_t GetEntitiesCount() const { return m_Entities.size(); }
		inline bool HasEntities() const { return m_Entities.size() != 0; }
		Entity& AddEntity(const std::string& EntityName);
		Entity* GetEntityByName(const std::string& EntityName);

	private:
		std::vector<Entity*> m_Entities;
	};
}