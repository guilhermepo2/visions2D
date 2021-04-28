#pragma once
#include <GL/glew.h>
#include <string>

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