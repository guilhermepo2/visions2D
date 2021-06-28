#pragma once
#include <visions2D.h>


class ObstacleComponent : public visions2D::Component {
public:

	void BeginPlay() override {
		m_Transform = Owner->GetComponentOfType<visions2D::TransformComponent>();
	}

	void Update(float DeltaTime) override {
		if (m_Transform->Position.x <= m_OutOfScreenToTheLeft) {
			// TODO: "Rand" should be a module in the engine.
			float NewY = visions2D::Random::Value() * m_MaxObstacleY;
			m_Transform->Position.x = m_OutOfScreenToTheRight;
			m_Transform->Position.y = NewY;
		}

		m_Transform->Translate(glm::vec2(m_Velocity * DeltaTime, 0.0f));
	}

private:
	float m_Velocity = -150.0f;
	float m_OutOfScreenToTheRight = 390.0f;
	float m_OutOfScreenToTheLeft = -390.0f;
	float m_MaxObstacleY = 90.0f;
	visions2D::TransformComponent* m_Transform;
};