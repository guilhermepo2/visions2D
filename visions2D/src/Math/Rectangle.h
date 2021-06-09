#pragma once
#include <glm/glm.hpp>
namespace visions2D {
	namespace Math {
		class Rectangle {
		public:
			Rectangle() {}
			Rectangle(float Left, float Top, float Width, float Height) : m_Left(Left), m_Top(Top), m_Width(Width), m_Height(Height) {}

			// TODO: static MakeRectangle

			float Left() const { return m_Left; }
			float Top() const { return m_Top; }
			float Width() const { return m_Width; }
			float Height() const { return m_Height; }

			float& Left() { return m_Left; }
			float& Top() { return m_Top; }
			float& Width() { return m_Width; }
			float& Height() { return m_Height; }

			float Right() const { return m_Left + m_Width; }
			float Bottom() const { return m_Top + m_Height; }

			glm::vec2 Position() const {
				return glm::vec2(m_Left, m_Top);
			}

			glm::vec2 Size() const {
				return glm::vec2(m_Width, m_Height);
			}

			glm::vec2 MaxPoint() const {
				return glm::vec2(m_Left + m_Width, m_Top);
			}

			glm::vec2 MinPoint() const {
				return glm::vec2(m_Left, m_Top + m_Height);
			}
			// extents?

			void SetPosition(float left, float top) {
				m_Left = left;
				m_Top = top;
			}

			void SetPosition(const glm::vec2& Position) {
				SetPosition(Position.x, Position.y);
			}

			void SetSize(float Width, float Height) {
				m_Width = Width;
				m_Height = Height;
			}

			void SetSize(const glm::vec2& Size) {
				SetSize(Size.x, Size.y);
			}

			// extents?
			// operator==
			// operator !=
			// operator +

			// is empty
			// center

			
			bool Contains(float x, float y) {
				return (x >= Left() && x <= Right() && y >= Top() && y <= Bottom());
			}

			bool Contains(glm::vec2 Position) {
				return Contains(Position.x, Position.y);
			}

			// intersects
			// inflate
			// contract
			// offset
			// constrain
			// clip to
			// operator RECT()?

		protected:
			float m_Left;
			float m_Top;
			float m_Width;
			float m_Height;
		};
	}
}