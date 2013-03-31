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
		float getSigned2DTriangleArea(Vector2f mA, Vector2f mB, Vector2f mC) { return (mA.x - mC.x) * (mB.y - mC.y) - (mA.y - mC.y) * (mB.x - mC.x); }
		bool isSegmentInsersecting(Vector2f mA1, Vector2f mA2, Vector2f mB1, Vector2f mB2, Vector2f& mIntersection)
		{
			float a1{getSigned2DTriangleArea(mA1, mA2, mB2)}, a2{getSigned2DTriangleArea(mA1, mA2, mB1)};
			if(a1 * a2 >= 0.f) return false;
				
			float a3{getSigned2DTriangleArea(mB1, mB2, mA1)}, a4{a3 + a2 - a1};
			if(a3 * a4 >= 0.f) return false;
			
			mIntersection = mA1 + (a3 / (a3 - a4)) * (mA2 - mA1);
			return true;
		}
		
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
