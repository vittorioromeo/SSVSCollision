// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Body/Body.h"
#include "AABB/AABB.h"

namespace ssvsc
{
	namespace Utils
	{
		template<typename T, typename V> bool contains(const T& mItems, const V& mValue) { return std::find(std::begin(mItems),std::end(mItems), mValue) != std::end(mItems); }
		template<typename T, typename T2> bool containsAny(T& mItems, const T2& mValues) { for(auto& value : mValues) if(contains(mItems, value)) return true; return false; }
		template<typename T, typename V> void eraseRemove(T& mItems, const V& mValue) { mItems.erase(std::remove(std::begin(mItems), std::end(mItems), mValue), std::end(mItems)); }
		template<typename T, typename P> void sort(T& mItems, P mPredicate) { std::sort(std::begin(mItems), std::end(mItems), mPredicate); }

		int getIntersectionX(const AABB& mA, const AABB& mB);
		int getIntersectionY(const AABB& mA, const AABB& mB);
		sf::Vector2i getIntersection(const AABB& mA, const AABB& mB);
		sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB);
		int getOverlapArea(const AABB& mA, const AABB& mB);
		bool isOverlapping(const AABB& mA, const AABB& mB);

		AABB getMergedAABBHorizontally(const AABB& mA, const AABB& mB);
		AABB getMergedAABBVertically(const AABB& mA, const AABB& mB);

		std::vector<AABB> getMergedAABBSLeft(std::vector<AABB> mSource);
		std::vector<AABB> getMergedAABBSRight(std::vector<AABB> mSource);
		std::vector<AABB> getMergedAABBSTop(std::vector<AABB> mSource);
		std::vector<AABB> getMergedAABBSBottom(std::vector<AABB> mSource);
	}
}

#endif // UTILS_H
