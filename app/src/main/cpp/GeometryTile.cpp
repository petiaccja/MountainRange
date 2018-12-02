#include "GeometryTile.hpp"

namespace mrange {


GeometryTile::GeometryTile(int subdivisions) {
	auto [vertices, indices] = MakeTriangulation(Vec2(1.f, 1.f), subdivisions);
	m_vertices = std::move(vertices);
	m_indices = std::move(indices);
}


const std::vector<VertexPNT>& GeometryTile::GetVertices() const {
	return m_vertices;
}


const std::vector<uint32_t>& GeometryTile::GetIndices() const {
	return m_indices;
}


void GeometryTile::ApplyTransform(const Mat44& transform) {
	for (auto& vertex : m_vertices) {
		vertex.position = vertex.position*transform;
	}
}


void GeometryTile::ApplyCurvature(float radius, Vec3 origin, Vec3 normal) {
	// TODO: this is not finished

	normal.Normalize();
	Vec3 center = origin - radius*normal;
	Hyperplane<float, 3> plane(origin, normal);
	float circumference = 2*radius*Constants<float>::Pi;

	for (auto& vertex : m_vertices) {
		Vec3& pos = vertex.position;
		float elevation = plane.Distance(pos); // How far the point is from the plane vertically.
		Vec3 offset = pos - elevation*normal; // Origin + offset is the point "under" pos, on the plane.
		float distance = offset.Length();
		float angle = circumference/distance;
		Vec3 axis = Cross(offset, normal).SafeNormalized();
		Vec3 radial = normal*Quat::AxisAngle(axis, angle);

	}
}


void GeometryTile::ApplyHeightmap(const Image<uint16_t>& heightmap, float minLevel, float maxLevel) {
	for (auto& vertex : m_vertices) {
		float height = heightmap.SampleBilinear(vertex.texcoord);
		vertex.position.z += height / std::numeric_limits<uint16_t>::max() * (maxLevel-minLevel) - minLevel;
	}
}


void GeometryTile::RecomputeNormals() {

}


auto GeometryTile::MakeTriangulation(Vec2 size, int subdivisions)
	-> std::pair<std::vector<VertexPNT>, std::vector<uint32_t>>
{
	std::vector<VertexPNT> vertices;
	std::vector<uint32_t> indices;

	// Generate vertices.
	for (uint32_t y = 0; y <= subdivisions; ++y) {
		for (uint32_t x = 0; x <= subdivisions; ++x) {
			Vec3 normal = { 0,0,1 };
			Vec2 pos = { x, y };
			Vec2 texcoord = pos / float(subdivisions);
			pos = texcoord - Vec2(0.5f, 0.5f);
			pos *= size;
			vertices.push_back({ pos | 0, normal, texcoord });
		}
	}

	// Generate triangles.
	auto LinearizeIndex = [subdivisions](Vec2u index) -> uint32_t {
		return index.y*subdivisions + index.x;
	};
	for (uint32_t y = 0; y < subdivisions; ++y) {
		for (uint32_t x = 0; x < subdivisions; ++x) {
			Vec2u quadIdx = { x,y };
			Vec2u vertIdx[4] = {
					quadIdx,
					quadIdx + Vec2i{0, 1},
					quadIdx + Vec2i{1, 1},
					quadIdx + Vec2i{1, 0},
			};

			// One triangle.
			indices.push_back(LinearizeIndex(vertIdx[0]));
			indices.push_back(LinearizeIndex(vertIdx[1]));
			indices.push_back(LinearizeIndex(vertIdx[2]));

			// Another triangle.
			indices.push_back(LinearizeIndex(vertIdx[0]));
			indices.push_back(LinearizeIndex(vertIdx[2]));
			indices.push_back(LinearizeIndex(vertIdx[3]));
		}
	}

	return { vertices, indices };
}


} // namespace mrange