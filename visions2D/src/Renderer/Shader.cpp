#include "Shader.h"
#include <Log.h>
#include <fstream>
#include <sstream>

namespace visions2D {
	Shader::Shader() : m_ShaderProgram(0), m_VertexShader(0), m_FragmentShader(0) {}
	Shader::~Shader() {}

	bool Shader::Load(const std::string& _vert, const std::string& _frag) {
		if (
			!CompileShader(_vert, GL_VERTEX_SHADER, m_VertexShader) ||
			!CompileShader(_frag, GL_FRAGMENT_SHADER, m_FragmentShader)
			) {
			return false;
		}

		m_ShaderProgram = glCreateProgram();
		glAttachShader(m_ShaderProgram, m_VertexShader);
		glAttachShader(m_ShaderProgram, m_FragmentShader);
		glLinkProgram(m_ShaderProgram);

		if (!IsValidProgram()) {
			return false;
		}

		return true;
	}

	void Shader::Unload() {
		glDeleteProgram(m_ShaderProgram);
		glDeleteShader(m_VertexShader);
		glDeleteShader(m_FragmentShader);
	}

	void Shader::SetActive() {
		glUseProgram(m_ShaderProgram);
	}

	bool Shader::CompileShader(const std::string& _Filename, GLenum _ShaderType, GLuint& _OutShader) {
		std::ifstream ShaderFile(_Filename);

		if (ShaderFile.is_open()) {
			std::stringstream sstream;
			sstream << ShaderFile.rdbuf();
			std::string contents = sstream.str();
			const char* ContentsChar = contents.c_str();

			_OutShader = glCreateShader(_ShaderType);
			glShaderSource(_OutShader, 1, &ContentsChar, nullptr);
			glCompileShader(_OutShader);

			if (!IsCompiled(_OutShader)) {
				LOG_ERROR("[shader] failed to compile shader: {0}", _Filename.c_str());
				return false;
			}
		}
		else {
			LOG_ERROR("[shader] shader file not found: {0}", _Filename.c_str());
			return false;
		}

		return true;
	}

	bool Shader::IsCompiled(GLuint Shader) {
		GLint Status;
		glGetShaderiv(Shader, GL_COMPILE_STATUS, &Status);

		if (Status != GL_TRUE) {
			char Buffer[512];
			memset(Buffer, 0, 512);
			glGetShaderInfoLog(Shader, 511, nullptr, Buffer);
			LOG_WARNING("[shader] GLSL compile failed: {0}", Buffer);
			return false;
		}

		return true;
	}

	bool Shader::IsValidProgram() {
		GLint Status;
		glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &Status);

		if (Status != GL_TRUE) {
			char Buffer[512];
			memset(Buffer, 0, 512);
			glGetProgramInfoLog(m_ShaderProgram, 511, nullptr, Buffer);
			LOG_WARNING("[shader] SDL Link Status: {0}", Buffer);
			return false;
		}

		return true;
	}
}