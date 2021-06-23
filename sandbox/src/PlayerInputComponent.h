#pragma once
#include <visions2D.h>

class PlayerInput : public visions2D::Component {
public:
	void BeginPlay() override {
		m_TransformReference = Owner->GetComponentOfType<visions2D::TransformComponent>();
		visions2D::BoxCollider* b = Owner->GetComponentOfType<visions2D::BoxCollider>();

		// TODO: Have an easier way to bind the callback!
		if (b != nullptr) {
			b->OnCollisionEnter = COLLISION_CALLBACK(&PlayerInput::PlayerOnCollisionEnter);
			b->OnCollisionExit = COLLISION_CALLBACK(&PlayerInput::PlayerOnCollisionExit);
		}
	}

	void PlayerOnCollisionEnter(visions2D::BoxCollider* Other) {
		if (Other->GetTag() == "obstacle") {
			LOG_INFO("Game Over!");
		}
		else if (Other->GetTag() == "point-collider") {
			m_Points++;
		}
	}

	void PlayerOnCollisionExit(visions2D::BoxCollider* Other) {
		// LOG_INFO("Player OnCollisionExit with {0}", Other->Owner->Name);
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

	inline int GetPoints() const { return m_Points; }
private:
	visions2D::TransformComponent* m_TransformReference = nullptr;
	float m_UpForce = 225.0f;
	float m_RotationSpeed = 100.0f;
	float m_VerticalVelocity = 0.0f;
	float m_Gravity = 500.0f;

	int m_Points = 0;
};