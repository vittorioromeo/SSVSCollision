#ifndef UTILS_H
#define UTILS_H

#include <vector>

namespace ssvsc
{
	namespace Utils
	{
		template<typename T> bool contains(const std::vector<T>& mVector, const T& mValue) { return find(begin(mVector), end(mVector), mValue) != end(mVector); }
		template<typename T> bool containsAny(const std::vector<T>& mVector, const std::vector<T>& mValues) { for(auto& value : mValues) if(contains(mVector, value)) return true; return false; }
		template<typename T> void eraseFromVector(std::vector<T>& mVector, const T& mItem) { mVector.erase(std::remove(std::begin(mVector), std::end(mVector), mItem), std::end(mVector)); }
	}
}

#endif // UTILS_H
