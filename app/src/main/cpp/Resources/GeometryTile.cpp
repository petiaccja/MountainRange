#include "GeometryTile.hpp"

namespace mrange {


GeometryTile::GeometryTile(int subdivisions) {
	m_gridWidth = subdivisions;
	m_gridHeight = subdivisions;
	auto [vertices, indices] = MakeTriangulation(Vec2(1.f, 1.f), subdivisions, subdivisions);
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


void GeometryTile::ApplyHeightmap(const Image<float>& heightmap) {
	for (auto& vertex : m_vertices) {
		float height = heightmap.SampleBilinear(vertex.texcoord);
		vertex.position.z += height;
	}
}


void GeometryTile::RecomputeNormals() {
	std::vector<Vec3> normals;
	normals.resize(m_vertices.size(), Vec3(0.0f));

	// Add up all triangle normals and average them.
	for (size_t i=0; i<m_indices.size(); i+=3) {
		int i0 = m_indices[i];
		int i1 = m_indices[i+1];
		int i2 = m_indices[i+2];
		const Vec3& p0 = m_vertices[i0].position;
		const Vec3& p1 = m_vertices[i1].position;
		const Vec3& p2 = m_vertices[i2].position;
		Vec3 normal = Cross(p0 - p1, p2 - p0);
		normal.Normalize();
		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (size_t i=0; i<normals.size(); ++i) {
		m_vertices[i].normal = normals[i].Normalized();
	}
}


auto GeometryTile::MakeTriangulation(Vec2 size, size_t gridWidth, size_t gridHeight)
	-> std::pair<std::vector<VertexPNT>, std::vector<uint32_t>>
{
	std::vector<VertexPNT> vertices;
	std::vector<uint32_t> indices;

	// Generate vertices.
	for (uint32_t y = 0; y <= gridHeight; ++y) {
		for (uint32_t x = 0; x <= gridWidth; ++x) {
			Vec3 normal = { 0,0,1 };
			Vec2 pos = { x, y };
			Vec2 texcoord = pos / Vec2(float(gridWidth), float(gridHeight));
			pos = texcoord - Vec2(0.5f, 0.5f);
			pos *= size;
			vertices.push_back({ pos | 0, normal, texcoord });
		}
	}

	// Generate triangles.
	auto LinearizeIndex = [gridWidth, gridHeight](Vec2u index) -> uint32_t {
		return index.y*(gridWidth+1) + index.x;
	};
	for (uint32_t y = 0; y < gridHeight; ++y) {
		for (uint32_t x = 0; x < gridWidth; ++x) {
			Vec2u quadIdx = { x,y };
			Vec2u vertIdx[4] = {
					quadIdx,
					quadIdx + Vec2u{0, 1},
					quadIdx + Vec2u{1, 1},
					quadIdx + Vec2u{1, 0},
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

const VertexPNT &GeometryTile::GetVertex(size_t x, size_t y) const {
	assert(x < m_gridWidth);
	assert(y < m_gridHeight);
	return m_vertices[y*(m_gridWidth+1) + x];
}

VertexPNT &GeometryTile::GetVertex(size_t x, size_t y) {
	assert(x < m_gridWidth);
	assert(y < m_gridHeight);
	return m_vertices[y*(m_gridWidth+1) + x];;
}


} // namespace mrange