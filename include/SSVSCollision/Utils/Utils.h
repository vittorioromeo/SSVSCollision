// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS
#define SSVSC_UTILS

#include <vector>
#include <SFML/System.hpp>
#include "SSVSCollision/AABB/AABB.h"

namespace ssvsc
{
	struct ResolverBase;
	struct SpatialBase;

	namespace Utils
	{
		template<typename T, typename... TArgs> static ResolverBase& createResolver(TArgs&&... mArgs)	{ return *(new T(std::forward<TArgs>(mArgs)...)); }
		template<typename T, typename... TArgs> static SpatialBase& createSpatial(TArgs&&... mArgs)		{ return *(new T(std::forward<TArgs>(mArgs)...)); }

		template<typename T> inline T getSigned2DTriangleArea(const sf::Vector2<T>& mA, const sf::Vector2<T>& mB, const sf::Vector2<T>& mC)
		{
			return (mA.x - mC.x) * (mB.y - mC.y) - (mA.y - mC.y) * (mB.x - mC.x);
		}
		template<typename T> inline bool isSegmentInsersecting(const Segment<T>& mA, const Segment<T>& mB, sf::Vector2<T>& mIntersection)
		{
			float a1{getSigned2DTriangleArea(mA.start, mA.end, mB.end)}, a2{getSigned2DTriangleArea(mA.start, mA.end, mB.start)};
			if(a1 * a2 > 0.f) return false;

			float a3{getSigned2DTriangleArea(mB.start, mB.end, mA.start)}, a4{a3 + a2 - a1};
			if(a3 * a4 > 0.f) return false;

			mIntersection = mA.start + (a3 / (a3 - a4)) * (mA.end - mA.start);
			return true;
		}

		inline int getMinIntersectionX(const AABB& mA, const AABB& mB)
		{
			int left{mB.getLeft() - mA.getRight()}, right{mB.getRight() - mA.getLeft()};
			return abs(left) < abs(right) ? left : right;
		}
		inline int getMinIntersectionY(const AABB& mA, const AABB& mB)
		{
			int top{mB.getTop() - mA.getBottom()}, bottom{mB.getBottom() - mA.getTop()};
			return abs(top) < abs(bottom) ? top : bottom;
		}
		inline sf::Vector2i getMin1DIntersection(const AABB& mA, const AABB& mB)
		{
			int iX{getMinIntersectionX(mA, mB)}, iY{getMinIntersectionY(mA, mB)};
			return abs(iX) < abs(iY) ? sf::Vector2i{iX, 0} : sf::Vector2i{0, iY};
		}
		inline sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB)	{ return {getMinIntersectionX(mA, mB), getMinIntersectionY(mA, mB)}; }
		inline int getOverlapX(const AABB& mA, const AABB& mB)					{ return mA.getLeft() < mB.getLeft() ? mA.getRight() - mB.getLeft() : mB.getRight() - mA.getLeft(); }
		inline int getOverlapY(const AABB& mA, const AABB& mB)					{ return mA.getTop() < mB.getTop() ? mA.getBottom() - mB.getTop() : mB.getBottom() - mA.getTop(); }
		inline int getOverlapArea(const AABB& mA, const AABB& mB)				{ return getOverlapX(mA, mB) * getOverlapY(mA, mB); }

	}
}

#endif
