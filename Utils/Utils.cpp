#include "Utils.h"

namespace ssvsc
{
	namespace Utils
	{
		int getOverlapX(Body* mA, Body* mB) { return mA->getLeft() < mB->getLeft() ? mA->getRight() - mB->getLeft() : mB->getRight() - mA->getLeft(); }
		int getOverlapY(Body* mA, Body* mB) { return mA->getTop() < mB->getTop() ? mA->getBottom() - mB->getTop() : mB->getBottom() - mA->getTop(); }
		int getOverlapArea(Body* mA, Body* mB) { return getOverlapX(mA, mB) * getOverlapY(mA, mB); }
		bool overlapAreaComparer(Body* mBody, Body* mA, Body* mB) { return getOverlapArea(mBody, mA) > getOverlapArea(mBody, mB); }
		bool isOverlapping(Body* mA, Body* mB) { return mA->getRight() > mB->getLeft() && mA->getLeft() < mB->getRight() && (mA->getBottom() > mB->getTop() && mA->getTop() < mB->getBottom()); }
	}
}
