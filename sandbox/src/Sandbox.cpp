// *******************************************************
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

bool bRenderCollision = false;

void Start() {
	inputSystem = new visions2D::InputSystem();
	gameWorld = new visions2D::GameWorld();
	collisionWorld = new visions2D::CollisionWorld();
	if (collisionWorld->Initialize()) {
		LOG_INFO("app: collision world initialized");
	}

	// load assets here...

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

void Render(visions2D::Renderer* RendererReference) {
	gameWorld->Render();

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