#include "Control.h"

namespace visions2D {
	bool Control::ProcessInput(const InputState& CurrentInputState) {
		glm::vec2 MousePosition = CurrentInputState.Mouse.GetPosition();
		bool MouseLeftButtonPressed = CurrentInputState.Mouse.IsMouseKeyDown(v2D_Mousecode::MOUSECODE_LEFT);
		
		if (m_Rect.Contains(MousePosition)) {
			
			if (!m_MouseOver) {
				if (m_OnMouseEnter != nullptr) {
					m_OnMouseEnter();
				}

				m_MouseOver = true;
			}

			if (!m_Focused && MouseLeftButtonPressed) {
				
				if (m_OnFocusReceived != nullptr) {
					m_OnFocusReceived();
				}

				m_Focused = true;
				return true;
			}

		}
		else {
			if (m_MouseOver) {
				
				if (m_OnMouseEnter != nullptr) {
					m_OnMouseEnter();
				}

				m_MouseOver = false;
			}

			if (m_Focused && MouseLeftButtonPressed) {
				if (m_OnFocusLost != nullptr) {
					m_OnFocusLost();
				}

				m_Focused = false;
			}
		}

		return false;
	}

	void Control::Update(float DeltaTime) {
		m_ForegroundColor.rgba[3] = m_Alpha;
		m_BackgroundColor.rgba[3] = m_Alpha;
	}

	void Control::Render() {
		if (CustomDrawCall != nullptr) {
			CustomDrawCall();
		}
		else {
			InternalDraw();
		}
	}

	void Control::InternalDraw() { // do nothing? }
}