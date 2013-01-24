#ifndef UTILS_H
#define UTILS_H

#include <vector>

namespace ssvsc
{
	namespace Utils
	{
		template<typename T, typename V> bool contains(const T& mVector, const V& mValue) { return std::find(std::begin(mVector),std::end(mVector), mValue) != std::end(mVector); }
		template<typename T, typename T2> bool containsAny(T& mVector, const T2& mValues) { for(auto& value : mValues) if(contains(mVector, value)) return true; return false; }
		template<typename T, typename V> void eraseFromVector(T& mVector, const V& mItem) { mVector.erase(std::remove(std::begin(mVector), std::end(mVector), mItem), std::end(mVector)); }
	}
}

#endif // UTILS_H
