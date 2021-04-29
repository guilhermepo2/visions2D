#include "VertexArray.h"
#include <GL/glew.h>


namespace visions2D {
	VertexArray::VertexArray(const float* _verts, unsigned int _vertPropertiesCount, unsigned int _numVerts, const unsigned int* _indices, unsigned int _numIndices)
	: m_NumberOfIndices(_numIndices), m_NumberOfVertices(_numVerts) {

		glGenVertexArrays(1, &m_VertexArrayID);
		glBindVertexArray(m_VertexArrayID);

		glGenBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, _numVerts * (_vertPropertiesCount * sizeof(float)), _verts, GL_STATIC_DRAW);

		glGenBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numIndices * sizeof(unsigned int), _indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// texture coords
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
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