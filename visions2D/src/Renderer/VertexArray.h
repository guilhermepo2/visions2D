#pragma once

namespace visions2D {
	class VertexArray {
	public:
		VertexArray(const float* _verts, unsigned int _vertPropertiesCount, unsigned int _numVerts, const unsigned int* _indices, unsigned int _numIndices);
		~VertexArray();

		void SetActive();

		inline unsigned int GetNumberOfIndices() const { return m_NumberOfIndices; }
		inline unsigned int GetNumberOfVertices() const { return m_NumberOfVertices; }

	private:
		unsigned int m_NumberOfVertices;
		unsigned int m_NumberOfIndices;

		unsigned int m_VertexBufferID;
		unsigned int m_IndexBufferID;
		unsigned int m_VertexArrayID;
	};
}