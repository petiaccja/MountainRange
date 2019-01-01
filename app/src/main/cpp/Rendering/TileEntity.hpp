#pragma once


#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <InlineMath.hpp>

#include "../Resources/GeometryTile.hpp"



namespace mrange {


class TileEntity {
public:
	void SetTile(const GeometryTile& tile);

	GLuint GetVertexBuffer() const;
	GLuint GetIndexBuffer() const;
	unsigned GetIndexCount() const;
private:
	void Clear();

private:
	GLuint m_vertexBuffer = 0;
	GLuint m_indexBuffer = 0;
	unsigned m_indexCount = 0;
};



} // namespace mrange