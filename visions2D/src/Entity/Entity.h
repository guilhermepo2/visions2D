#pragma once
#include <string>
#include <vector>
#include <map>
#include <typeinfo>

namespace visions2D {
	class Component;
	struct InputState;

	class Entity {
	public:
		std::string Name;
		Entity();
		Entity(const std::string& InName);
		inline bool IsActive() const { return m_bIsActive; }

	private:
		bool m_bIsActive;
		// TODO: Instead of every entity having a references to the components, maybe make
		// the components reference the entities, and have components array?
		// since we don't really care about the entities
		std::vector<Component*> m_Components;
		std::map<const std::type_info*, Component*> m_TypeInfoToComponentMap;

	public:
		void BeginPlay();
		bool ProcessInput(const InputState& CurrentInputState);
		void Update(float DeltaTime);
		// TODO: Do I really need this function here?
		void Render();
		void Destroy();

		template<typename T, typename... TArgs>
		T& AddComponent(TArgs&&... Args) {
			T* NewComponent(new T(std::forward<TArgs>(Args)...));
			NewComponent->Owner = this;
			m_Components.push_back(NewComponent);
			m_TypeInfoToComponentMap[&typeid(*NewComponent)] = NewComponent;
			NewComponent->Initialize();
			return *NewComponent;
		}

		template<typename T>
		T* GetComponentOfType() {
			return static_cast<T*>(m_TypeInfoToComponentMap[&typeid(T)]);
		}

		template<typename T>
		bool HasComponentOfType() const {
			return m_TypeInfoToComponentMap.count(&typeid(T));
		}
	};
}