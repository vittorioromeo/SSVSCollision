// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Utils/Utils.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	namespace Utils
	{		
		int getMinIntersectionX(const AABB& mA, const AABB& mB)
		{
			int left{mB.getLeft() - mA.getRight()}, right{mB.getRight() - mA.getLeft()};
			return abs(left) < abs(right) ? left : right;
		}
		int getMinIntersectionY(const AABB& mA, const AABB& mB)
		{
			int top{mB.getTop() - mA.getBottom()}, bottom{mB.getBottom() - mA.getTop()};
			return abs(top) < abs(bottom) ? top : bottom;
		}
		Vector2i getMinIntersection(const AABB& mA, const AABB& mB) { return {getMinIntersectionX(mA, mB), getMinIntersectionY(mA, mB)}; }
		Vector2i getMin1DIntersection(const AABB& mA, const AABB& mB)
		{
			int iX{getMinIntersectionX(mA, mB)}, iY{getMinIntersectionY(mA, mB)};
			return abs(iX) < abs(iY) ? Vector2i{iX, 0} : Vector2i{0, iY};
		}
		int getOverlapX(const AABB& mA, const AABB& mB) { return mA.getLeft() < mB.getLeft() ? mA.getRight() - mB.getLeft() : mB.getRight() - mA.getLeft(); }
		int getOverlapY(const AABB& mA, const AABB& mB) { return mA.getTop() < mB.getTop() ? mA.getBottom() - mB.getTop() : mB.getBottom() - mA.getTop(); }
    	int getOverlapArea(const AABB& mA, const AABB& mB) { return getOverlapX(mA, mB) * getOverlapY(mA, mB); }
		bool isOverlapping(const AABB& mA, const AABB& mB) { return mA.getRight() > mB.getLeft() && mA.getLeft() < mB.getRight() && (mA.getBottom() > mB.getTop() && mA.getTop() < mB.getBottom()); }
	}
}
