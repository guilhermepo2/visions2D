﻿// *******************************************************
// *******************************************************
// 
// 
// 
// *******************************************************
// *******************************************************

#include "visions2D.h"

visions2D::InputSystem* inputSystem = nullptr;
visions2D::GameWorld* gameWorld = nullptr;
visions2D::CollisionWorld* collisionWorld = nullptr;

visions2D::Texture* PlayerTexture = nullptr;
// "Player"
visions2D::Entity* PlayerEntity = nullptr;
float m_UpForce = 325.0f;
float RotationSpeed = 100.0f;
float m_VerticalVelocity = 0.0f;
float m_Gravity = 500.0f;

bool bRenderCollision = false;

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();
	collisionWorld = new visions2D::CollisionWorld();
	if (collisionWorld->Initialize()) {
		LOG_INFO("app: collision world initialized");
	}

	// load assets here...
	// loading textures
	PlayerTexture = new visions2D::Texture();
	PlayerTexture->Load("./src/DefaultAssets/White.png");

	// creating entities
	PlayerEntity = gameWorld->AddEntity("player-entity");
	PlayerEntity->AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	visions2D::SpriteComponent& spriteComponent = PlayerEntity->AddComponent<visions2D::SpriteComponent>(PlayerTexture, 0);
	spriteComponent.SpriteColor.SetColor(0.0f, 1.0f, 0.0f, 1.0f);

	inputSystem->Initialize();
	gameWorld->BeginPlay();
}

void PreInput() {
	inputSystem->PrepareForUpdate();
}

void Input() {
	inputSystem->Update();
	gameWorld->ProcessInput(inputSystem->GetState());

	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_SPACE)) {
		LOG_INFO("spacebar was pressed and the input system works!");
	}

	if (inputSystem->GetState().Keyboard.WasKeyPressedThisFrame(visions2D::v2D_Keycode::KEYCODE_I)) {
		bRenderCollision = !bRenderCollision;
	}

	if (inputSystem->GetState().Mouse.WasMouseKeyPressedThisFrame(visions2D::v2D_Mousecode::MOUSECODE_LEFT)) {
		m_VerticalVelocity = m_UpForce;
	}
}

void Update(float DeltaTime) {
	gameWorld->Update(DeltaTime);

	// Player Update (should move into its own component)
	m_VerticalVelocity -= DeltaTime * m_Gravity;
	PlayerEntity->GetComponentOfType<visions2D::TransformComponent>()->Translate(glm::vec2(0.0f, m_VerticalVelocity * DeltaTime));

	if (m_VerticalVelocity <= 0.0f) {
		float CurrentRotation = PlayerEntity->GetComponentOfType<visions2D::TransformComponent>()->Rotation;
		CurrentRotation = glm::min(CurrentRotation + (1.0f * DeltaTime * RotationSpeed), 45.0f);
		PlayerEntity->GetComponentOfType<visions2D::TransformComponent>()->Rotation = CurrentRotation;
	}
	else {
		PlayerEntity->GetComponentOfType<visions2D::TransformComponent>()->Rotation = -30.0f;
	}

	collisionWorld->VerifyAllCollisions();
}

// TODO: RendererReference here because the reference is needed to push the draw information.
// I want to keep this here because I can substitute it for the batch.
// So the engine gives the application a drawing batch
void Render(visions2D::Renderer* RendererReference) {
	gameWorld->Render(RendererReference);

	if (bRenderCollision) {
		collisionWorld->Render(RendererReference);
	}
}

void OnImGui() {

}

void Shutdown() {
	gameWorld->Destroy();
	delete gameWorld;

	inputSystem->Shutdown();
	delete inputSystem;

	collisionWorld->Shutdown();
	delete collisionWorld;
}

int main(void) {

	visions2D::AppConfig conf;
	conf.WindowName = "Flappy Thing";
	conf.Width = 1024;
	conf.Height = 576;

	conf.Startup = Start;
	conf.PreProcessInput = PreInput;
	conf.ProcessInput = Input;
	conf.ImGuiRender = OnImGui;
	conf.Update = Update;
	conf.Render = Render;

	// maybe I should have a static Application::Run(AppConfig) ?!
	visions2D::Application* app = new visions2D::Application(conf);
	app->Run();
	delete app;

	return 0;
}