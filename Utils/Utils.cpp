#include "Utils.h"

namespace ssvsc
{
	namespace Utils
	{
		int getIntersectionX(Body* mA, Body* mB)
		{
			int left{mB->getLeft() - mA->getRight()}, right{mB->getRight() - mA->getLeft()};
			return abs(left) < abs(right) ? left : right;
		}
		int getIntersectionY(Body* mA, Body* mB)
		{
			int top{mB->getTop() - mA->getBottom()}, bottom{mB->getBottom() - mA->getTop()};
			return abs(top) < abs(bottom) ? top : bottom;
		}
		int getOverlapArea(Body* mA, Body* mB) { return abs(getIntersectionX(mA, mB)) * abs(getIntersectionY(mA, mB)); }
		bool isOverlapping(Body* mA, Body* mB) { return mA->getRight() > mB->getLeft() && mA->getLeft() < mB->getRight() && (mA->getBottom() > mB->getTop() && mA->getTop() < mB->getBottom()); }
	}
}
