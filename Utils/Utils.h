// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS
#define SSVSC_UTILS

#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "AABB/AABB.h"

namespace ssvsc
{
	class ResolverBase;
	class SpatialBase;

	namespace Utils
	{
		template<typename TResolver, typename... TArgs> static ResolverBase& createResolver(TArgs&&... mArgs) { return *(new TResolver(std::forward<TArgs>(mArgs)...)); }
		template<typename TSpatial, typename... TArgs> static SpatialBase& createSpatial(TArgs&&... mArgs) { return *(new TSpatial(std::forward<TArgs>(mArgs)...)); }
		
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
