#pragma once
#include <SDL_scancode.h>
#include <SDL_gamecontroller.h>
#include <SDL_mouse.h>
#include <vector>

#include "KeyboardInput.h"
#include "MouseInput.h"
#include <glm/glm.hpp>

union SDL_Event;

namespace visions2D {
	enum EButtonState {
		ENone,
		EPressed,
		EReleased,
		EHeld
	};

	// -------------------------------------------
	// Keyboard State
	// -------------------------------------------
	class KeyboardState {
	public:
		friend class InputSystem;
		bool GetKeyValue(v2D_Keycode KeyCode) const;
		EButtonState GetKeyState(v2D_Keycode KeyCode) const;

		bool IsKeyDown(v2D_Keycode KeyCode) const;
		bool WasKeyPressedThisFrame(v2D_Keycode KeyCode) const;
		bool WasKeyReleasedThisFrame(v2D_Keycode KeyCode) const;

	private:
		const Uint8* m_CurrentState;
		Uint8 m_PreviousState[NUM_SCANCODES];
	};

	// -------------------------------------------
	// Mouse State
	// -------------------------------------------
	class MouseState {
	public:
		friend class InputSystem;
		const glm::vec2& GetPosition() const { return m_MousePosition; }

		bool GetButtonValue(v2D_Mousecode MouseKey) const;
		EButtonState GetButtonState(v2D_Mousecode MouseKey) const;

		bool IsMouseKeyDown(v2D_Mousecode MouseKey) const;
		bool WasMouseKeyPressedThisFrame(v2D_Mousecode MouseKey) const;
		bool WasKMouseKeyReleasedThisFrame(v2D_Mousecode MouseKey) const;


	private:
		glm::vec2 m_MousePosition;
		glm::vec2 m_ScrollWheel;
		Uint32 m_MouseButtons;
		Uint32 m_MouseButtonsLastFrame;
		bool m_IsRelative;
	};

	// -------------------------------------------
	// Game Controller State
	// -------------------------------------------
	class ControllerState {
	public:
		friend class InputSystem;
		bool GetButtonValue(SDL_GameControllerButton Button) const;
		EButtonState GetButtonState(SDL_GameControllerButton Button) const;

		bool GetIscConnected()		const { return m_IsConnected; }
		float GetLeftTrigger()		const { return m_LeftTrigger; }
		float GetRightTrigger()		const { return m_RightTrigger; }
		glm::vec2 GetLeftStick()		const { return m_LeftStick; }
		glm::vec2 GetRightStick()		const { return m_RightStick; }

	private:
		Uint8 m_CurrentButtons[SDL_CONTROLLER_BUTTON_MAX];
		Uint8 m_PreviousButtons[SDL_CONTROLLER_BUTTON_MAX];
		bool m_IsConnected;

		float m_LeftTrigger;
		float m_RightTrigger;

		glm::vec2 m_LeftStick;
		glm::vec2 m_RightStick;
	};

	// -------------------------------------------
	// Input State
	// -------------------------------------------
	struct InputState {
		KeyboardState	Keyboard;
		MouseState		Mouse;
		ControllerState Controller;
	};

	// -------------------------------------------
	// Input System
	// -------------------------------------------
	class InputSystem {
	public:
		bool Initialize();
		void Shutdown();
		void PrepareForUpdate();
		void Update();

		void SetRelativeMouseMode(bool Value);
		void ProcessEvent(SDL_Event& Event);
		const InputState& GetState() const { return m_State; }

	private:
		float Filter1D(int Input);
		glm::vec2 Filter2D(int InputX, int InputY);
		InputState m_State;
		SDL_GameController* m_Controller;
	};
}