#pragma once

#include <boost/algorithm/string.hpp>
#include <boost/assert.hpp>

namespace nemo
{

namespace {
	constexpr auto left   = "left";
	constexpr auto right  = "right";
	constexpr auto top    = "top";
	constexpr auto bottom = "bottom";
	constexpr auto center = "center";
}

enum class HzAlign
{
	Left,
	Center,
	Right
};

enum class VtAlign
{
	Top,
	Center,
	Bottom
};

struct AlignConfig
{
	HzAlign hz_;
	VtAlign vt_;

	AlignConfig(
		const HzAlign hz = HzAlign::Left, 
		const VtAlign vt = VtAlign::Center
	)
		: hz_(hz)
		, vt_(vt)
	{
	}

	AlignConfig(const std::string& hz, const std::string& vt)
	{
		BOOST_ASSERT(hz == left || hz == center || hz == right);
		BOOST_ASSERT(vt == top || vt == center || vt == bottom);

		hz_ = boost::iequals(hz, left) 
			? HzAlign::Left : boost::iequals(hz, right)
				? HzAlign::Right : HzAlign::Center;
		
		vt_ = boost::iequals(vt, top)
			? VtAlign::Top : boost::iequals(vt, bottom)
				? VtAlign::Bottom : VtAlign::Center;
	}
};

}