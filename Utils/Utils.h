#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <algorithm>
#include "Body/Body.h"

namespace ssvsc
{
	namespace Utils
	{
		template<typename T, typename V> bool contains(const T& mItems, const V& mValue) { return std::find(std::begin(mItems),std::end(mItems), mValue) != std::end(mItems); }
		template<typename T, typename T2> bool containsAny(T& mItems, const T2& mValues) { for(auto& value : mValues) if(contains(mItems, value)) return true; return false; }
		template<typename T, typename V> void eraseRemove(T& mItems, const V& mValue) { mItems.erase(std::remove(std::begin(mItems), std::end(mItems), mValue), std::end(mItems)); }
		template<typename T, typename P> void sort(T& mItems, P mPredicate) { std::sort(std::begin(mItems), std::end(mItems), mPredicate); }

		int getOverlapX(Body* mA, Body* mB);
		int getOverlapY(Body* mA, Body* mB);
		int getOverlapArea(Body* mA, Body* mB);
		bool overlapAreaComparer(Body* mBody, Body* mA, Body* mB);
		bool isOverlapping(Body* mA, Body* mB);
	}
}

#endif // UTILS_H
