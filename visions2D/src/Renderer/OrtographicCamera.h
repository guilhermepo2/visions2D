#pragma once
#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace visions2D {
	class OrtographicCamera {

	public:

		OrtographicCamera(float _width, float _height) {
			m_CameraProjection = glm::ortho(
				-(_width / 2.0f), (_width / 2.0f),
				-(_height / 2.0f), (_height / 2.0f),
				-10.0f, 10.0f
			);

			m_CameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 10.0f));

			m_CameraView = glm::inverse(m_CameraTransform);
		}

		glm::mat4 GetCameraViewProjection() {
			return m_CameraProjection * m_CameraView;
		}

		void SetColor(float _r, float _g, float _b, float _a) {
			CameraBackgroundColor.rgba[0] = _r;
			CameraBackgroundColor.rgba[1] = _g;
			CameraBackgroundColor.rgba[2] = _b;
			CameraBackgroundColor.rgba[3] = _a;
		}

		Color CameraBackgroundColor;

	private:
		glm::mat4 m_CameraProjection;
		glm::mat4 m_CameraTransform;
		glm::mat4 m_CameraView;
	};
}