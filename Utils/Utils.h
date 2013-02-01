#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <algorithm>
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
		int getOverlapArea(const AABB& mA, const AABB& mB);
		bool isOverlapping(const AABB& mA, const AABB& mB);
	}
}

#endif // UTILS_H
