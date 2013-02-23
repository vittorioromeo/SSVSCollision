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
	namespace Utils
	{
		template<typename T, typename V> bool contains(const T& mItems, const V& mValue) { return std::find(std::begin(mItems),std::end(mItems), mValue) != std::end(mItems); }
		template<typename T, typename T2> bool containsAny(T& mItems, const T2& mValues) { for(auto& value : mValues) if(contains(mItems, value)) return true; return false; }
		template<typename T, typename V> void eraseRemove(T& mItems, const V& mValue) { mItems.erase(std::remove(std::begin(mItems), std::end(mItems), mValue), std::end(mItems)); }
		template<typename T, typename P> void sort(T& mItems, P mPredicate) { std::sort(std::begin(mItems), std::end(mItems), mPredicate); }

		template<typename TMergeTraits> std::vector<AABB> getMergedAABBs(std::vector<AABB> mSource)
		{
			std::vector<AABB> result;
			while(!mSource.empty())
			{
				bool merged{false}; AABB a{mSource.back()}; mSource.pop_back();
				for(auto& b : mSource)
				{
					if(TMergeTraits::side(a) != TMergeTraits::side(b)) continue;
					result.push_back(TMergeTraits::merge(a, b));
					eraseRemove(mSource, b); merged = true; break;					
				}
				if(!merged) result.push_back(a);
			}
			return result;
		}

		AABB getMergedAABBHorizontally(const AABB& mA, const AABB& mB);
		AABB getMergedAABBVertically(const AABB& mA, const AABB& mB);
		int getMinIntersectionX(const AABB& mA, const AABB& mB);
		int getMinIntersectionY(const AABB& mA, const AABB& mB);
		sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB);
		sf::Vector2i getMin1DIntersection(const AABB& mA, const AABB& mB);
		int getOverlapArea(const AABB& mA, const AABB& mB);
		bool isOverlapping(const AABB& mA, const AABB& mB);
	}
}

#endif // SSVSC_UTILS_H
