// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS_H
#define SSVSC_UTILS_H

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

		template<typename T, typename V> bool contains(const T& mItems, const V& mValue) { return std::find(std::begin(mItems),std::end(mItems), mValue) != std::end(mItems); }
		template<typename T, typename T2> bool containsAny(T& mItems, const T2& mValues) { for(auto& value : mValues) if(contains(mItems, value)) return true; return false; }
		template<typename T, typename V> void eraseRemove(T& mItems, const V& mValue) { mItems.erase(std::remove(std::begin(mItems), std::end(mItems), mValue), std::end(mItems)); }
		template<typename T, typename P> void sort(T& mItems, P mPredicate) { std::sort(std::begin(mItems), std::end(mItems), mPredicate); }

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

#endif // SSVSC_UTILS_H
