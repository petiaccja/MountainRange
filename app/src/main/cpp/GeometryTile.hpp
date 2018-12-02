#pragma once


#include "Vertex.hpp"
#include "Image.hpp"

#include <vector>
#include <utility>


namespace mrange {


class GeometryTile {
public:
	GeometryTile(int subdivisions);

	// Access geometry.
	const std::vector<VertexPNT>& GetVertices() const;
	const std::vector<uint32_t>& GetIndices() const;

	// Process geometry.
	void ApplyTransform(const Mat44& transform);
	void ApplyCurvature(float radius, Vec3 origin, Vec3 normal);
	void ApplyHeightmap(const Image<uint16_t>& heightmap, float minLevel, float maxLevel);
	void RecomputeNormals();

private:
	static std::pair<std::vector<VertexPNT>, std::vector<uint32_t>> MakeTriangulation(Vec2 size, int subdivisions);

private:
	std::vector<VertexPNT> m_vertices;
	std::vector<uint32_t> m_indices;
};


} // namespace mrange