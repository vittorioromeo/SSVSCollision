#ifndef TRAITS_H
#define TRAITS_H

#include "Utils/Utils.h"

namespace ssvsc
{
	namespace Traits
	{
		namespace Merge
		{
			struct Left
			{
				static int side(const AABB& mAABB) { return mAABB.getRight(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return Utils::getMergedAABBVertically(mA, mB); }
			};

			struct Right
			{
				static int side(const AABB& mAABB) { return mAABB.getLeft(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return Utils::getMergedAABBVertically(mA, mB); }
			};

			struct Top
			{
				static int side(const AABB& mAABB) { return mAABB.getBottom(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return Utils::getMergedAABBHorizontally(mA, mB); }
			};

			struct Bottom
			{
				static int side(const AABB& mAABB) { return mAABB.getTop(); }
				static AABB merge(const AABB& mA, const AABB& mB) { return Utils::getMergedAABBHorizontally(mA, mB); }
			};
		}
	}
}

#endif // TRAITS_H
