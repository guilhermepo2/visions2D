#pragma once

namespace visions2D {

	class Color;

	class VertexArray {
	public:
		VertexArray(
			const float* _verts, unsigned int _vertPropertiesCount, unsigned int _numVerts, 
			const float* _texCoords,
			const unsigned int* _indices, unsigned int _numIndices);
		~VertexArray();

		void SetActive();
		void SubTexCoords(const float* _texCoords);
		void SubColorCoords(const float* _Color);
		void SubColorCoords(Color _Color);

		inline unsigned int GetNumberOfIndices() const { return m_NumberOfIndices; }
		inline unsigned int GetNumberOfVertices() const { return m_NumberOfVertices; }

	private:
		unsigned int m_NumberOfVertices;
		unsigned int m_NumberOfIndices;

		unsigned int m_VertexBufferID;
		unsigned int m_TexCoordsBufferID;
		unsigned int m_ColorBufferID;

		unsigned int m_IndexBufferID;

		unsigned int m_VertexArrayID;
	};
}