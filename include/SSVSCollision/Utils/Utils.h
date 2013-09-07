// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS
#define SSVSC_UTILS

#include <vector>
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/AABB/AABB.h"

namespace ssvsc
{
	struct ResolverBase;
	struct SpatialBase;

	namespace Utils
	{
		template<typename T, typename... TArgs> static ResolverBase& createResolver(TArgs&&... mArgs)	{ return *(new T(std::forward<TArgs>(mArgs)...)); }
		template<typename T, typename... TArgs> static SpatialBase& createSpatial(TArgs&&... mArgs)		{ return *(new T(std::forward<TArgs>(mArgs)...)); }

		template<typename T> inline T getSigned2DTriangleArea(const Vec2<T>& mA, const Vec2<T>& mB, const Vec2<T>& mC) noexcept
		{
			return (mA.x - mC.x) * (mB.y - mC.y) - (mA.y - mC.y) * (mB.x - mC.x);
		}
		template<typename T> inline bool isSegmentInsersecting(const Segment<T>& mA, const Segment<T>& mB, Vec2<T>& mIntersection) noexcept
		{
			float a1{getSigned2DTriangleArea(mA.start, mA.end, mB.end)}, a2{getSigned2DTriangleArea(mA.start, mA.end, mB.start)};
			if(a1 * a2 > 0.f) return false;

			float a3{getSigned2DTriangleArea(mB.start, mB.end, mA.start)}, a4{a3 + a2 - a1};
			if(a3 * a4 > 0.f) return false;

			mIntersection = mA.start + (a3 / (a3 - a4)) * (mA.end - mA.start);
			return true;
		}

		template<typename T> inline T getMinAbs(const T& mA, const T& mB) noexcept { return abs(mA) < abs(mB) ? mA : mB; }

		inline int getMinIntersectionX(const AABB& mA, const AABB& mB) noexcept { return getMinAbs(mB.getLeft() - mA.getRight(), mB.getRight() - mA.getLeft()); }
		inline int getMinIntersectionY(const AABB& mA, const AABB& mB) noexcept { return getMinAbs(mB.getTop() - mA.getBottom(), mB.getBottom() - mA.getTop()); }
		inline Vec2i getMin1DIntersection(const AABB& mA, const AABB& mB) noexcept
		{
			int iX{getMinIntersectionX(mA, mB)}, iY{getMinIntersectionY(mA, mB)};
			return abs(iX) < abs(iY) ? Vec2i{iX, 0} : Vec2i{0, iY};
		}
		inline Vec2i getMinIntersection(const AABB& mA, const AABB& mB) noexcept	{ return {getMinIntersectionX(mA, mB), getMinIntersectionY(mA, mB)}; }
		inline int getOverlapX(const AABB& mA, const AABB& mB) noexcept				{ return mA.getLeft() < mB.getLeft() ? mA.getRight() - mB.getLeft() : mB.getRight() - mA.getLeft(); }
		inline int getOverlapY(const AABB& mA, const AABB& mB) noexcept				{ return mA.getTop() < mB.getTop() ? mA.getBottom() - mB.getTop() : mB.getBottom() - mA.getTop(); }
		inline int getOverlapArea(const AABB& mA, const AABB& mB) noexcept			{ return getOverlapX(mA, mB) * getOverlapY(mA, mB); }

	}
}

#endif
