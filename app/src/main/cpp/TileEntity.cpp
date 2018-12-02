#include "TileEntity.hpp"
#include "Vertex.hpp"


namespace mrange {


void TileEntity::SetTile(const GeometryTile& tile) {
	Clear();
	glGetError(); // Clear error flag.

	GLuint vbuffer = 0;
	GLuint ibuffer = 0;
	glGenBuffers(1, &vbuffer); // Create vertex buffer.
	glGenBuffers(1, &ibuffer); // Create index buffer.

	// Pack vertices.
	const std::vector<VertexPNT>& tileVertices = tile.GetVertices();
	std::vector<VertexPNT_Packed> packedVertices;
	packedVertices.reserve(tileVertices.size());
	for (auto& tileVertex : tileVertices) {
		packedVertices.push_back({tileVertex.position, tileVertex.normal, tileVertex.texcoord});
	}

	// Get indices.
	const std::vector<uint32_t>& indices = tile.GetIndices();


	// Upload vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbuffer);
	glBufferData(GL_ARRAY_BUFFER, packedVertices.size()*sizeof(VertexPNT_Packed), packedVertices.data(), GL_STATIC_DRAW);

	// Upload indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	m_indexCount = (unsigned)indices.size();

	if (glGetError() != GL_NO_ERROR) {
		throw std::runtime_error("Failed to create vertex and index buffers.");
	}

	m_vertexBuffer = vbuffer;
	m_indexBuffer = ibuffer;
}


GLuint TileEntity::GetVertexBuffer() const {
	return m_vertexBuffer;
}


GLuint TileEntity::GetIndexBuffer() const {
	return m_indexBuffer;
}


unsigned TileEntity::GetIndexCount() const {
	return m_indexCount;
}


void TileEntity::Clear() {
	if (m_vertexBuffer) {
		glDeleteBuffers(1, &m_vertexBuffer);
	}
	if (m_indexBuffer) {
		glDeleteBuffers(1, &m_indexBuffer);
	}
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	m_indexCount = 0;
}





}  // namespace mrange