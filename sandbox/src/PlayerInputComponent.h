#pragma once
#include <visions2D.h>

class PlayerInput : public visions2D::Component {
public:
	void BeginPlay() override {
		m_TransformReference = Owner->GetComponentOfType<visions2D::TransformComponent>();
	}

	bool ProcessInput(const visions2D::InputState& CurrentInputState) override {

		if (CurrentInputState.Mouse.WasMouseKeyPressedThisFrame(visions2D::v2D_Mousecode::MOUSECODE_LEFT)) {
			m_VerticalVelocity = m_UpForce;
			return true;
		}

		return false;
	}

	void Update(float DeltaTime) override {
		m_VerticalVelocity -= DeltaTime * m_Gravity;
		m_TransformReference->Translate(glm::vec2(0.0f, m_VerticalVelocity * DeltaTime));

		if (m_VerticalVelocity <= 0.0f) {
			float CurrentRotation = m_TransformReference->Rotation;
			CurrentRotation = glm::min(CurrentRotation + (1.0f * DeltaTime * m_RotationSpeed), 45.0f);
			m_TransformReference->Rotation = CurrentRotation;
		}
		else {
			m_TransformReference->Rotation = -30.0f;
		}
	}
private:
	visions2D::TransformComponent* m_TransformReference = nullptr;
	float m_UpForce = 325.0f;
	float m_RotationSpeed = 100.0f;
	float m_VerticalVelocity = 0.0f;
	float m_Gravity = 500.0f;
};