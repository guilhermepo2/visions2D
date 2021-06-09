#pragma once
#include "Common.h"
#include "Entity/Component/Component.h"
#include "Entity/Component/TransformComponent.h"
#include "Math/Rectangle.h"
#include <string>
#include <glm/glm.hpp>
#include "Collision/CollisionWorld.h"
#include "Entity/Entity.h"

using CollisionCallbackFunction = std::function<void(visions2D::BoxCollider*)>;

namespace visions2D {
	class BoxCollider : public Component {
	public:
		BoxCollider(const glm::vec2 MinPoint, const glm::vec2 MaxPoint) {
			m_Rect.SetPosition(MinPoint.x, MaxPoint.y);
			m_Rect.SetSize(MaxPoint.x - MinPoint.x, MaxPoint.y - MinPoint.y);
			CollisionWorld::s_Instance->AddColliderToWorld(this);
		}

		BoxCollider(const std::string ColliderTag, const glm::vec2& MinPoint, const glm::vec2& MaxPoint) : BoxCollider(MinPoint, MaxPoint) {
			m_ColliderTag = ColliderTag;
		}

		~BoxCollider() {
			CollisionWorld::s_Instance->RemoveColliderFromWorld(this);
		}

		virtual void BeginPlay() {
			m_TransformReference = Owner->GetComponentOfType<TransformComponent>();
		}

		void HandleCollisionCallback(BoxCollider* Other) {
			if (CollisionCallback != nullptr) {
				CollisionCallback(Other);
			}
		}

		CollisionCallbackFunction CollisionCallback;
		const std::string& GetTag() const { return m_ColliderTag; }

		Math::Rectangle GetScaledRect() {
			Math::Rectangle t_rect;
			t_rect.SetPosition(glm::vec2(m_Rect.Left(), m_Rect.Top()));
			t_rect.SetSize(glm::vec2(m_Rect.Width() * m_TransformReference->Scale.x, m_Rect.Height() * m_TransformReference->Scale.y));
			return t_rect;
		}

		Math::Rectangle GetWorldPositionRectangle() {
			Math::Rectangle t_rect = GetScaledRect();
			t_rect.SetPosition(
				t_rect.Left() + m_TransformReference->Position.x,
				t_rect.Top() + m_TransformReference->Position.y
			);

			return t_rect;
		}

	private:
		Math::Rectangle m_Rect;
		TransformComponent* m_TransformReference;
		std::string m_ColliderTag;
	};
}