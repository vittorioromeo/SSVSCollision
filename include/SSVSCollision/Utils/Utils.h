// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS
#define SSVSC_UTILS

#include <vector>
#include <algorithm>
#include <SFML/System.hpp>
#include "SSVSCollision/AABB/AABB.h"

namespace ssvsc
{
	class ResolverBase;
	class SpatialBase;

	namespace Utils
	{
		template<typename TResolver, typename... TArgs> static ResolverBase& createResolver(TArgs&&... mArgs) { return *(new TResolver(std::forward<TArgs>(mArgs)...)); }
		template<typename TSpatial, typename... TArgs> static SpatialBase& createSpatial(TArgs&&... mArgs) { return *(new TSpatial(std::forward<TArgs>(mArgs)...)); }

		template<typename T> T getSigned2DTriangleArea(const sf::Vector2<T>& mA, const sf::Vector2<T>& mB, const sf::Vector2<T>& mC)
		{
			return (mA.x - mC.x) * (mB.y - mC.y) - (mA.y - mC.y) * (mB.x - mC.x);
		}
		template<typename T> bool isSegmentInsersecting(const Segment<T>& mA, const Segment<T>& mB, sf::Vector2<T>& mIntersection)
		{
			float a1{getSigned2DTriangleArea(mA.start, mA.end, mB.end)}, a2{getSigned2DTriangleArea(mA.start, mA.end, mB.start)};
			if(a1 * a2 > 0.f) return false;

			float a3{getSigned2DTriangleArea(mB.start, mB.end, mA.start)}, a4{a3 + a2 - a1};
			if(a3 * a4 > 0.f) return false;

			mIntersection = mA.start + (a3 / (a3 - a4)) * (mA.end - mA.start);
			return true;
		}

		int getMinIntersectionX(const AABB& mA, const AABB& mB);
		int getMinIntersectionY(const AABB& mA, const AABB& mB);
		sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB);
		sf::Vector2i getMin1DIntersection(const AABB& mA, const AABB& mB);
		int getOverlapX(const AABB& mA, const AABB& mB);
		int getOverlapY(const AABB& mA, const AABB& mB);
		int getOverlapArea(const AABB& mA, const AABB& mB);
		bool isOverlapping(const AABB& mA, const AABB& mB);
	}
}

#endif
