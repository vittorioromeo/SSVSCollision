// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS
#define SSVSC_UTILS

#include "SSVSCollision/Global/Typedefs.hpp"
#include "SSVSCollision/AABB/AABB.hpp"

namespace ssvsc
{
	namespace Utils
	{
		template<typename T> inline constexpr T getSigned2DTriangleArea(const Vec2<T>& mA, const Vec2<T>& mB, const Vec2<T>& mC) noexcept
		{
			return (mA.x - mC.x) * (mB.y - mC.y) - (mA.y - mC.y) * (mB.x - mC.x);
		}
		template<typename T> inline bool isSegmentInsersecting(const Segment<T>& mA, const Segment<T>& mB) noexcept
		{
			float a1(getSigned2DTriangleArea(mA.start, mA.end, mB.end)), a2(getSigned2DTriangleArea(mA.start, mA.end, mB.start));
			if(a1 * a2 > 0.f) return false;

			float a3(getSigned2DTriangleArea(mB.start, mB.end, mA.start)), a4{a3 + a2 - a1};
			return a3 * a4 > 0.f || a3 - a4 == 0;
		}
		template<typename T> inline bool isSegmentInsersecting(const Segment<T>& mA, const Segment<T>& mB, Vec2<T>& mIntersection) noexcept
		{
			float a1(getSigned2DTriangleArea(mA.start, mA.end, mB.end)), a2(getSigned2DTriangleArea(mA.start, mA.end, mB.start));
			if(a1 * a2 > 0.f) return false;

			float a3(getSigned2DTriangleArea(mB.start, mB.end, mA.start)), a4{a3 + a2 - a1};
			if(a3 * a4 > 0.f || a3 - a4 == 0) return false;

			mIntersection = mA.start + (a3 / (a3 - a4)) * (mA.end - mA.start);
			return true;
		}
	}
}

#endif
