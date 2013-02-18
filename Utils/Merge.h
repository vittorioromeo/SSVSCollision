#ifndef MERGE_H
#define MERGE_H

#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "Body/Body.h"
#include "Utils.h"

namespace ssvsc
{
	namespace Utils
	{
		AABB getMergedAABBHorizontally(const AABB& mA, const AABB& mB);
		AABB getMergedAABBVertically(const AABB& mA, const AABB& mB);

		template<typename TMergeTraits> std::vector<AABB> getMergedAABBs(std::vector<AABB> mSource)
		{
			std::vector<AABB> result;

			while(!mSource.empty())
			{
				bool merged{false}; AABB a{mSource.back()}; mSource.pop_back();

				for(auto& b : mSource)
				{
					if(TMergeTraits::side(a) == TMergeTraits::side(b))
					{
						result.push_back(TMergeTraits::merge(a, b));
						eraseRemove(mSource, b); merged = true; break;
					}
				}

				if(!merged) result.push_back(a);
			}

			return result;
		}

		namespace Side
		{
			struct Left
			{
				static int side(const AABB& mAABB) { return mAABB.getRight(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return getMergedAABBVertically(mA, mB); }
			};

			struct Right
			{
				static int side(const AABB& mAABB) { return mAABB.getLeft(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return getMergedAABBVertically(mA, mB); }
			};

			struct Top
			{
				static int side(const AABB& mAABB) { return mAABB.getBottom(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return getMergedAABBHorizontally(mA, mB); }
			};

			struct Bottom
			{
				static int side(const AABB& mAABB) { return mAABB.getTop(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return getMergedAABBHorizontally(mA, mB); }
			};
		}
	}
}

#endif // MERGE_H
