#include "Utils.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	namespace Utils
	{
		int getIntersectionX(const AABB& mA, const AABB& mB)
		{
			int left{mB.getLeft() - mA.getRight()}, right{mB.getRight() - mA.getLeft()};
			return abs(left) < abs(right) ? left : right;
		}
		int getIntersectionY(const AABB& mA, const AABB& mB)
		{
			int top{mB.getTop() - mA.getBottom()}, bottom{mB.getBottom() - mA.getTop()};
			return abs(top) < abs(bottom) ? top : bottom;
		}
		Vector2i getMinIntersection(const AABB& mA, const AABB& mB)
		{
			int iX{getIntersectionX(mA, mB)}, iY{getIntersectionY(mA, mB)};
			if(abs(iX) < abs(iY)) return {iX, 0};
			return {0, iY};
		}
		int getOverlapArea(const AABB& mA, const AABB& mB) { return abs(getIntersectionX(mA, mB)) * abs(getIntersectionY(mA, mB)); }
		bool isOverlapping(const AABB& mA, const AABB& mB) { return mA.getRight() > mB.getLeft() && mA.getLeft() < mB.getRight() && (mA.getBottom() > mB.getTop() && mA.getTop() < mB.getBottom()); }

		AABB getMergedAABBHorizontally(const AABB& mA, const AABB& mB)
		{
			int x{min(mA.getX(), mB.getX()) + abs(mA.getX() - mB.getX())};
			Vector2i halfSize{mA.getHalfWidth() + mB.getHalfWidth(), mA.getHalfHeight()};
			return {{x, mA.getY()}, halfSize};
		}

		AABB getMergedAABBVertically(const AABB& mA, const AABB& mB)
		{
			int y{min(mA.getY(), mB.getY()) + abs(mA.getY() - mB.getY())};
			Vector2i halfSize{mA.getHalfWidth(), mA.getHalfHeight() + mB.getHalfHeight()};
			return {{mA.getX(), y}, halfSize};
		}

		vector<AABB> getMergedAABBSLeft(vector<AABB> mSource)
		{
			vector<AABB> result;

			while(!mSource.empty())
			{
				bool merged{false}; AABB a{mSource.back()}; mSource.pop_back();

				for(auto& b : mSource)
					if(a.getRight() == b.getRight())
					{
						result.push_back(getMergedAABBVertically(a, b));
						eraseRemove(mSource, b); merged = true; break;
					}
					
				if(!merged) result.push_back(a);
			}

			return result;
		}

		vector<AABB> getMergedAABBSRight(vector<AABB> mSource)
		{
			vector<AABB> result;

			while(!mSource.empty())
			{
				bool merged{false}; AABB a{mSource.back()}; mSource.pop_back();

				for(auto& b : mSource)
					if(a.getLeft() == b.getLeft())
					{
						result.push_back(getMergedAABBVertically(a, b));
						eraseRemove(mSource, b); merged = true; break;
					}

				if(!merged) result.push_back(a);
			}

			return result;
		}

		vector<AABB> getMergedAABBSTop(vector<AABB> mSource)
		{
			vector<AABB> result;

			while(!mSource.empty())
			{
				bool merged{false}; AABB a{mSource.back()}; mSource.pop_back();

				for(auto& b : mSource)
					if(a.getBottom() == b.getBottom())
					{
						result.push_back(getMergedAABBHorizontally(a, b));
						eraseRemove(mSource, b); merged = true; break;
					}

				if(!merged) result.push_back(a);
			}

			return result;
		}

		vector<AABB> getMergedAABBSBottom(vector<AABB> mSource)
		{
			vector<AABB> result;

			while(!mSource.empty())
			{
				bool merged{false}; AABB a{mSource.back()}; mSource.pop_back();

				for(auto& b : mSource)
					if(a.getTop() == b.getTop())
					{
						result.push_back(getMergedAABBHorizontally(a, b));
						eraseRemove(mSource, b); merged = true; break;
					}

				if(!merged) result.push_back(a);
			}

			return result;
		}
	}
}
