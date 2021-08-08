#pragma once
#include <string>

namespace visions2D {
	class Window {
	public:
		inline unsigned int GetScreenWidth() const { return m_ScreenWidth; }
		inline unsigned int GetScreenHeight() const { return m_ScreenHeight; }
		inline std::string GetWindowTitle() const { return m_WindowTitle; }

		virtual void Swap() = 0;
		virtual void* GetPlatformSpecificWindow() const = 0;
		virtual void Destroy() = 0;

	protected:
		unsigned int m_ScreenWidth;
		unsigned int m_ScreenHeight;
		std::string m_WindowTitle;

	};
}