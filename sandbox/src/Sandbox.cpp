﻿// *******************************************************
// *******************************************************
// 
// 
// 
// *******************************************************
// *******************************************************

#include <visions2D.h>
#include "PlayerInputComponent.h"

#include <glm/gtc/random.hpp>

visions2D::InputSystem* inputSystem = nullptr;
visions2D::GameWorld* gameWorld = nullptr;
visions2D::CollisionWorld* collisionWorld = nullptr;

visions2D::Texture* PlayerTexture = nullptr;
visions2D::Texture* ObstacleTexture = nullptr;
// "Player"
visions2D::Entity* PlayerEntity = nullptr;
visions2D::Entity* ObstacleEntity = nullptr;
visions2D::Entity* ObstacleEntity2 = nullptr;

// TODO: "Points" should be a child of the Obstacles...
visions2D::Entity* PointsComponent = nullptr;
visions2D::Entity* PointsComponent2 = nullptr;

visions2D::Entity* LowerCollider = nullptr;
visions2D::Entity* UpperCollider = nullptr;
visions2D::Font* LazyTown = nullptr;

bool bRenderCollision = false;

float MaxObstacleY = 90.0f;
float MinObstacleY = -90.0f;
float ObstacleYPosition = 0.0f;

float OutOfScreenToTheRight = 390.0f;
float OutOfScreenToTheLeft = -390.0f;

class ObstacleComponent : public visions2D::Component {
public:

	void BeginPlay() override {
		m_Transform = Owner->GetComponentOfType<visions2D::TransformComponent>();
	}

	void Update(float DeltaTime) override {
		if (m_Transform->Position.x <= OutOfScreenToTheLeft) {
			float NewY = glm::linearRand(-1.0f, 1.0f) * MaxObstacleY;

			m_Transform->Position.x = OutOfScreenToTheRight;
			m_Transform->Position.y = NewY;
		}

		m_Transform->Translate(glm::vec2(m_Velocity * DeltaTime, 0.0f));
	}

private:
	float m_Velocity = -150.0f;
	visions2D::TransformComponent* m_Transform;
};

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();
	collisionWorld = new visions2D::CollisionWorld();
	if (collisionWorld->Initialize()) {
		LOG_INFO("app: collision world initialized");
	}

	// load assets here...
	LazyTown = new visions2D::Font();
	LazyTown->Load("./assets/ChevyRay - Lazytown.ttf");
	// loading textures
	PlayerTexture = new visions2D::Texture();
	PlayerTexture = LazyTown->RenderToTexture("@", 32);
	ObstacleTexture = LazyTown->RenderToTextureWrapped("xxx\nxxx\nxxx\nxxx\nxxx", 32, 128);
	// PlayerTexture->Load("./src/DefaultAssets/White.png");

	// creating entities
	PlayerEntity = gameWorld->AddEntity("player-entity");
	PlayerEntity->AddComponent<visions2D::TransformComponent>(glm::vec2(-150.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	visions2D::SpriteComponent& spriteComponent = PlayerEntity->AddComponent<visions2D::SpriteComponent>(PlayerTexture, 0);
	spriteComponent.SpriteColor.SetColor(0.0f, 1.0f, 0.0f, 1.0f);
	spriteComponent.FlipVertical = true;
	PlayerEntity->AddComponent<visions2D::BoxCollider>("PlayerCollider", glm::vec2(-16.0f, -16.0f), glm::vec2(16.0f, 16.0f));
	PlayerEntity->AddComponent<PlayerInput>();

	//
	ObstacleEntity = gameWorld->AddEntity("obstacle");
	ObstacleEntity->AddComponent<visions2D::TransformComponent>(glm::vec2(300.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	visions2D::SpriteComponent& obstacleSprite = ObstacleEntity->AddComponent<visions2D::SpriteComponent>(ObstacleTexture, 0);
	obstacleSprite.SpriteColor.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	// TODO: Add the option to offset a box collider
	// TODO: Make the box collider render the right box positions
	ObstacleEntity->AddComponent<visions2D::BoxCollider>("obstacle", glm::vec2(-56.0f, -75.0f), glm::vec2(56.0f, 75.0f));
	ObstacleEntity->AddComponent<ObstacleComponent>();

	PointsComponent = gameWorld->AddEntity("points1");
	PointsComponent->AddComponent<visions2D::TransformComponent>(glm::vec2(300.0f, 0.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	PointsComponent->AddComponent<visions2D::BoxCollider>("point-collider", glm::vec2(-5.0f, -300.0f), glm::vec2(5.0f, 300.0f));
	PointsComponent->AddComponent<ObstacleComponent>();

	//
	//
	ObstacleEntity2 = gameWorld->AddEntity("obstacle2");
	ObstacleEntity2->AddComponent<visions2D::TransformComponent>(glm::vec2(600.0f, 150.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	visions2D::SpriteComponent& obstacleSprite2 = ObstacleEntity2->AddComponent<visions2D::SpriteComponent>(ObstacleTexture, 0);
	obstacleSprite2.SpriteColor.SetColor(1.0f, 0.0f, 0.0f, 1.0f);
	// TODO: Add the option to offset a box collider
	// TODO: Make the box collider render the right box positions
	ObstacleEntity2->AddComponent<visions2D::BoxCollider>("obstacle", glm::vec2(-56.0f, -75.0f), glm::vec2(56.0f, 75.0f));
	ObstacleEntity2->AddComponent<ObstacleComponent>();

	UpperCollider = gameWorld->AddEntity("upper collider");
	UpperCollider->AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, 180.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	UpperCollider->AddComponent<visions2D::BoxCollider>("obstacle", glm::vec2(-320.0f, -5.0f), glm::vec2(320.0f, 5.0f));

	LowerCollider = gameWorld->AddEntity("lower collider");
	LowerCollider->AddComponent<visions2D::TransformComponent>(glm::vec2(0.0f, -180.0f), 0.0f, glm::vec2(1.0f, 1.0f));
	LowerCollider->AddComponent<visions2D::BoxCollider>("obstacle", glm::vec2(-320.0f, -5.0f), glm::vec2(320.0f, 5.0f));

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

	
}

void Update(float DeltaTime) {
	gameWorld->Update(DeltaTime);
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
	ImGui::Begin("Debug");
	int Points = PlayerEntity->GetComponentOfType<PlayerInput>()->GetPoints();
	ImGui::InputInt("Points", &Points);
	ImGui::End();

	/*
	ImGui::Begin("Debug");
	ImGui::InputFloat("Obstacle X", &ObstacleXPosition, 1.0f);
	ImGui::End();
	*/
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
	conf.WindowName = "Flappy ASCII";
	conf.Width = 640;
	conf.Height = 360;

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