#pragma once

#include <InlineMath.hpp>


namespace mrange {


struct VertexP {
	Vec3 position;
};

struct VertexPT {
	Vec3 position;
	Vec2 texcoord;
};

struct VertexPNT {
	Vec3 position;
	Vec3 normal;
	Vec2 texcoord;
};


struct VertexPNT_Packed {
	Vec3_Packed position;
	Vec3_Packed normal;
	Vec2_Packed texcoord;
};


} // namespace mrange