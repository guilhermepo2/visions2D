#pragma once
#include "Renderer/Color.h"
#include "Input/Input.h"
#include "Math/Rectangle.h"

#include <functional>
using ControlEvent = std::function<void(void)>;

namespace visions2D {

	class Control {
	public:
		bool ProcessInput(const InputState& CurrentInputState);
		void Update(float DeltaTime);
		virtual void Render();
		virtual void InternalDraw(); 

		// TODO: Make this a std::function?
		virtual void CustomDrawFunction();

		float& Alpha() { return m_Alpha; }
		const glm::vec2 Position() const { return m_Rect.Position(); }
		const glm::vec2 Size() const { return m_Rect.Size(); }
		void SetPosition(const glm::vec2& Position) { m_Rect.SetPosition(Position); }
		void SetSize(const glm::vec2& Size) { m_Rect.SetSize(Size); }
		const Math::Rectangle& Rectangle() { return m_Rect; }

		const Color& ForegroundColor() { return m_ForegroundColor; }
		const Color& BackgroundColor() { return m_BackgroundColor; }
		void SetForegroundColor(const Color& Color) { m_ForegroundColor = Color; } // call a function to refresh the control?
		void SetBackgroundColor(const Color& Color) { m_BackgroundColor = Color; } // call a function to refresh the control?

		void Show() { SetVisibility(true); }
		void Hide() { SetVisibility(false); }
		virtual void SetVisibility(bool show) { m_Visible = show; }

	protected:
		Math::Rectangle m_Rect;
		float m_Alpha;

		Color m_ForegroundColor;
		Color m_BackgroundColor;

		bool m_Visible;
		bool m_Enabled;
		bool m_Focused;
		bool m_MouseOver;

		// TODO: Viewport?

		ControlEvent m_OnFocusReceived;
		ControlEvent m_OnFocusLost;
		ControlEvent m_OnMouseEnter;
		ControlEvent m_OnMouseLeave;
	};
}