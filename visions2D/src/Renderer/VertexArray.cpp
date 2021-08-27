#include "VertexArray.h"
#include "Color.h"
#include <glad/glad.h>

/*
Vertex Shader Structure
location 0: vec2 position
location 1: vec2 TexCoords
location 2: vec4 vertexColors

TotalSize: 8
*/
namespace visions2D {

	VertexArray::VertexArray(
		const float* _verts, unsigned int _vertPropertiesCount, unsigned int _numVerts,
		const float* _texCoords,
		const unsigned int* _indices, unsigned int _numIndices)
	: m_NumberOfIndices(_numIndices), m_NumberOfVertices(_numVerts) {

		glGenVertexArrays(1, &m_VertexArrayID);
		glBindVertexArray(m_VertexArrayID);

		glGenBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, _numVerts * (_vertPropertiesCount * sizeof(float)), _verts, GL_STATIC_DRAW);
		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glGenBuffers(1, &m_TexCoordsBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordsBufferID);
		glBufferData(GL_ARRAY_BUFFER, _numVerts * (_vertPropertiesCount * sizeof(float)), _texCoords, GL_STATIC_DRAW);
		// texture coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);

		// color buffer
		glGenBuffers(1, &m_ColorBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_ColorBufferID);
		glBufferData(GL_ARRAY_BUFFER, _numVerts * (_vertPropertiesCount * sizeof(float)), nullptr, GL_STATIC_DRAW);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(2);

		glGenBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(unsigned int), _indices, GL_STATIC_DRAW);
	}

	void VertexArray::SubTexCoords(const float* _texCoords) {
		glBindBuffer(GL_ARRAY_BUFFER, m_TexCoordsBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * (2 * sizeof(float)), _texCoords);
	}

	void VertexArray::SubColorCoords(const float* _Color) {
		glBindBuffer(GL_ARRAY_BUFFER, m_ColorBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * (4 * sizeof(float)), _Color);
	}

	void VertexArray::SubColorCoords(Color _Color) {
		float Colors[] = {
			_Color.rgba[0], _Color.rgba[1], _Color.rgba[2], _Color.rgba[3],
			_Color.rgba[0], _Color.rgba[1], _Color.rgba[2], _Color.rgba[3],
			_Color.rgba[0], _Color.rgba[1], _Color.rgba[2], _Color.rgba[3],
			_Color.rgba[0], _Color.rgba[1], _Color.rgba[2], _Color.rgba[3]
		};
		SubColorCoords(static_cast<const float*>(Colors));
	}
	
	VertexArray::~VertexArray() {
		glDeleteBuffers(1, &m_VertexBufferID);
		glDeleteBuffers(1, &m_IndexBufferID);
		glDeleteBuffers(1, &m_VertexArrayID);
	}

	void VertexArray::SetActive() {
		glBindVertexArray(m_VertexArrayID);
	}
}