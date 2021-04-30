#pragma once
#include <GL/glew.h>
#include <string>
#include "Color.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace visions2D {
	class Shader {
	public:
		Shader();
		~Shader();

		bool Load(const std::string& _vert, const std::string& _frag);
		void Unload();
		void SetActive();
		GLuint GetShaderProgram() const { return m_ShaderProgram; }

		// TODO: set uniform, set matrix, set vector, set color......
		inline void SetVec4(const std::string& Name, float x, float y, float z, float w) 
		{
			glUniform4f(glGetUniformLocation(m_ShaderProgram, Name.c_str()), x, y, z, w);
		}

		inline void SetColor(const std::string& Name, Color _color) {
			SetVec4(Name, _color.rgba[0], _color.rgba[1], _color.rgba[2], _color.rgba[3]);
		}

		inline void SetMatrix4(const std::string& Name, glm::mat4 Mat) {
			glUniformMatrix4fv(
				glGetUniformLocation(m_ShaderProgram, Name.c_str()),
				1,
				GL_FALSE,
				glm::value_ptr(Mat)
			);
		}

	private:
		bool CompileShader(const std::string& _Filename, GLenum _ShaderType, GLuint& _OutShader);
		bool IsCompiled(GLuint Shader);
		bool IsValidProgram();

	private:
		GLuint m_VertexShader;
		GLuint m_FragmentShader;
		GLuint m_ShaderProgram;
	};
}