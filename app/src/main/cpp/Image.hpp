#pragma once

#include <InlineMath.hpp>

namespace mrange {


namespace impl {

template<class PixelT, bool IsVector = mathter::impl::IsVector<PixelT>::value>
struct Floatify {
	using type = float;
};

template<class PixelT>
struct Floatify<PixelT, true> {
	using type = Vector<float, PixelT::Dimension()>;
};

} // namespace impl


enum class eSampleMode {
	POINT,
	BILINEAR,
	BICUBIC,
};


template <class PixelT>
class Image {
	static_assert(mathter::impl::IsVector<PixelT>::value || std::is_fundamental_v<PixelT>);
	using FloatPixelT = typename impl::Floatify<PixelT>::type;
public:
	Image();
	Image(unsigned width, unsigned height);

	PixelT& operator()(unsigned x, unsigned y);
	const PixelT& operator()(unsigned x, unsigned y) const;
	PixelT& operator()(Vec2u coordinate);
	const PixelT& operator()(Vec2u coordinate) const;

	FloatPixelT SamplePoint(Vec2 coordinate) const;
	FloatPixelT SampleBilinear(Vec2 coordinate) const;
	FloatPixelT SampleBicubic(Vec2 coordinate) const;
	FloatPixelT Sample(Vec2 coordinate, eSampleMode mode) const;

private:
	unsigned m_width;
	unsigned m_height;
	std::vector<PixelT> m_pixels;
};


template<class PixelT>
Image<PixelT>::Image() {

}


template<class PixelT>
Image<PixelT>::Image(unsigned width, unsigned height) {
	m_width = width;
	m_height = height;
	m_pixels.resize(width*height);
}


template<class PixelT>
PixelT &Image<PixelT>::operator()(unsigned x, unsigned y) {
	assert(x < m_width);
	assert(y < m_height);
	return m_pixels[y*m_width + x];
}


template<class PixelT>
const PixelT &Image<PixelT>::operator()(unsigned x, unsigned y) const {
	assert(x < m_width);
	assert(y < m_height);
	return m_pixels[y*m_width + x];
}


template<class PixelT>
const PixelT &Image<PixelT>::operator()(Vec2u coordinate) const {
	return operator()(coordinate.x, coordinate.y);
}


template<class PixelT>
PixelT &Image<PixelT>::operator()(Vec2u coordinate) {
	return operator()(coordinate.x, coordinate.y);
}


template<class PixelT>
auto Image<PixelT>::SamplePoint(Vec2 coordinate) const -> FloatPixelT {
	coordinate = Max(Vec2(0,0), Min(Vec2(m_width, m_height), coordinate));
	Vec2u icoord = Vec2u(coordinate + 0.5f);
	return operator()(icoord);
}


template<class PixelT>
auto Image<PixelT>::SampleBilinear(Vec2 coordinate) const -> FloatPixelT {
	coordinate = Max(Vec2(0,0), Min(Vec2(m_width, m_height), coordinate));
	Vec2u c0 = Vec2u(coordinate);
	Vec2u c1 = c0 + Vec2u(1,0);
	Vec2u c2 = c0 + Vec2u(0,1);
	Vec2u c3 = c0 + Vec2u(1,1);

	Vec2u maxVec = { m_width-1, m_height-1 };
	c1 = Min(c1, maxVec);
	c2 = Min(c2, maxVec);
	c3 = Min(c3, maxVec);

	float t = coordinate.x - c0.x;
	float u = coordinate.y - c0.y;

	FloatPixelT bottom = (1-t)*FloatPixelT(operator()(c0)) + t*FloatPixelT(operator()(c1));
	FloatPixelT top = (1-t)*FloatPixelT(operator()(c2)) + t*FloatPixelT(operator()(c3));
	return (1-u)*bottom + t*top;
}


template<class PixelT>
auto Image<PixelT>::SampleBicubic(Vec2 coordinate) const -> FloatPixelT {
	throw std::logic_error("Not implemented.");
}


template<class PixelT>
auto Image<PixelT>::Sample(Vec2 coordinate, eSampleMode mode) const -> FloatPixelT  {
	switch (mode) {
		case eSampleMode::POINT: return SamplePoint(coordinate);
		case eSampleMode::BILINEAR: return SampleBilinear(coordinate);
		case eSampleMode::BICUBIC: return SampleBicubic(coordinate);
		default: assert(false);
	}
}


} // namespace