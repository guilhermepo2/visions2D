#pragma once
#include <glm/glm.hpp>
namespace visions2D {
	namespace Math {
		class Rectangle {
		public:
			Rectangle();
			Rectangle(float Left, float Top, float Width, float Height);

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
			// extents?

			void SetPosition(const glm::vec2& Position) {
				m_Left = Position.x;
				m_Top = Position.y;
			}

			void SetSize(const glm::vec2& Size) {
				m_Width = Size.x;
				m_Height = Size.y;
			}

			// extents?
			// operator==
			// operator !=
			// operator +

			// is empty
			// center

			// contains
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