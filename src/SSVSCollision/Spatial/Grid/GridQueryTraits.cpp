// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/GridQueryTraits.h"
#include "SSVSCollision/Spatial/Grid/GridQuery.h"

using namespace std;
using namespace sf;
using namespace ssvu;

namespace ssvsc
{
	namespace QueryTraits
	{
		namespace Bodies
		{
			void All::getBodies(Grid& mGrid, vector<Body*>& mBodies, const Vector2i& mIndex, const string&)
			{
				mBodies = mGrid.getCell(mIndex).getBodies();
			}

			void Grouped::getBodies(Grid& mGrid, vector<Body*>& mBodies, const Vector2i& mIndex, const string& mGroup)
			{
				mBodies = mGrid.getCell(mIndex).getBodies(mGroup);
			}

			void AllOffset::getBodies(Grid& mGrid, vector<Body*>& mBodies, const Vector2i& mIndex, const string&)
			{
				vector<Body*> result;

				for(int iY = -1; iY < 2; ++iY)
					for(int iX = -1; iX < 2; ++iX)
					{
						Vector2i index{mIndex + Vector2i(iX, iY)};
						if(!mGrid.isIndexValid(index)) continue;
						for(auto& b : mGrid.getCell(index).getBodies()) if(!ssvu::contains(result, b)) result.push_back(b);
					}

				mBodies = result;
			}

			void GroupedOffset::getBodies(Grid& mGrid, vector<Body*>& mBodies, const Vector2i& mIndex, const string& mGroup)
			{
				vector<Body*> result;

				for(int iY = -1; iY < 2; ++iY)
					for(int iX = -1; iX < 2; ++iX)
					{
						Vector2i index{mIndex + Vector2i(iX, iY)};
						if(!mGrid.isIndexValid(index)) continue;
						for(auto& b : mGrid.getCell(index).getBodies(mGroup)) if(!ssvu::contains(result, b)) result.push_back(b);
					}

				mBodies = result;
			}

		}

		namespace Orthogonal
		{
			bool Left::isValid(const GridQuery& mQuery) { return mQuery.getIndex().x >= mQuery.getGrid().getXMinIndex(); }
			void Left::step(GridQuery& mQuery) { mQuery.setIndexX(mQuery.getIndex().x - 1); }
			bool Left::getSorting(const GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().x < mB->getPosition().x; }
			bool Left::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getLeft() > mQuery.getPos().x || mQuery.getPos().y < mShape.getTop() || mQuery.getPos().y > mShape.getBottom(); }
			void Left::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mShape.getRight()); mQuery.setOutY(mQuery.getPos().y); }

			bool Right::isValid(const GridQuery& mQuery) { return mQuery.getIndex().x < mQuery.getGrid().getXMaxIndex(); }
			void Right::step(GridQuery& mQuery) { mQuery.setIndexX(mQuery.getIndex().x + 1); }
			bool Right::getSorting(const GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().x > mB->getPosition().x; }
			bool Right::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getRight() < mQuery.getPos().x || mQuery.getPos().y < mShape.getTop() || mQuery.getPos().y > mShape.getBottom(); }
			void Right::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mShape.getLeft()); mQuery.setOutY(mQuery.getPos().y); }

			bool Up::isValid(const GridQuery& mQuery) { return mQuery.getIndex().y >= mQuery.getGrid().getYMinIndex(); }
			void Up::step(GridQuery& mQuery) { mQuery.setIndexY(mQuery.getIndex().y - 1); }
			bool Up::getSorting(const GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().y < mB->getPosition().y; }
			bool Up::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getTop() > mQuery.getPos().y || mQuery.getPos().x < mShape.getLeft() || mQuery.getPos().x > mShape.getRight(); }
			void Up::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mQuery.getPos().x); mQuery.setOutY(mShape.getBottom()); }

			bool Down::isValid(const GridQuery& mQuery) { return mQuery.getIndex().y < mQuery.getGrid().getYMaxIndex(); }
			void Down::step(GridQuery& mQuery) { mQuery.setIndexY(mQuery.getIndex().y + 1); }
			bool Down::getSorting(const GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().y > mB->getPosition().y; }
			bool Down::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getBottom() < mQuery.getPos().y || mQuery.getPos().x < mShape.getLeft() || mQuery.getPos().x > mShape.getRight(); }
			void Down::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mQuery.getPos().x); mQuery.setOutY(mShape.getTop()); }
		}

		bool RayCast::isValid(const GridQuery& mQuery)
		{
			return mQuery.getGrid().isIndexValid(mQuery.getIndex());
		}
		void RayCast::step(GridQuery& mQuery)
		{
			const auto index(mQuery.getIndex());
			const auto startIndex(mQuery.getStartIndex());
			const auto deltaDist(mQuery.getDeltaDist());
			const auto sideDist(mQuery.getSideDist());
			const auto direction(mQuery.getDirection());
			const auto step(mQuery.getStep());
			
			mQuery.setPos(mQuery.getPos() + direction * static_cast<float>(mQuery.getGrid().getCellSize()));

			if(direction.x < 0)
			{
				mQuery.setStepX(-1);
				mQuery.setSideDistX((startIndex.x - index.x) * deltaDist.x);
			}
			else
			{
				mQuery.setStepX(1);
				mQuery.setSideDistX((index.x + 1.0f - startIndex.x) * deltaDist.x);
			}

			if(direction.y < 0)
			{
				mQuery.setStepY(-1);
				mQuery.setSideDistY((startIndex.y - index.y) * deltaDist.y);
			}
			else
			{
				mQuery.setStepY(1);
				mQuery.setSideDistY((index.y + 1.0f - startIndex.y) * deltaDist.y);
			}

			if(sideDist.x < sideDist.y)
			{
				mQuery.setSideDistX(sideDist.x + deltaDist.x);
				mQuery.setIndexX(index.x + step.x);
			}
			else
			{
				mQuery.setSideDistY(sideDist.y + deltaDist.y);
				mQuery.setIndexY(index.y + step.y);
			}
		}
		bool RayCast::getSorting(const GridQuery& mQuery, const Body* mA, const Body* mB)
		{
			Vector2f startPos{mQuery.getStartPos()};
			return sqrt(pow((mA->getPosition().x - startPos.x), 2) + pow((mA->getPosition().y - startPos.y), 2)) >
					sqrt(pow((mB->getPosition().x - startPos.x), 2) + pow((mB->getPosition().y - startPos.y), 2));
		}
		bool RayCast::misses(GridQuery& mQuery, const AABB& mShape)
		{
			vector<pair<Vector2i, Vector2i>> lines;
			lines.push_back({mShape.getNWCorner(), mShape.getNECorner()});
			lines.push_back({mShape.getNECorner(), mShape.getSECorner()});
			lines.push_back({mShape.getSECorner(), mShape.getSWCorner()});
			lines.push_back({mShape.getSWCorner(), mShape.getNWCorner()});

			bool f{false};
			float t;
			Vector2f intersection;

			while(!f && !lines.empty())
			{
				auto currentLine(lines.back());
				lines.pop_back();
				f = Test2DSegmentSegment(mQuery.getStartPos(), mQuery.getPos(), Vector2f(currentLine.first), Vector2f(currentLine.second), t, intersection);
			}

			if(!f) return true;
			else
			{
				mQuery.setOutX(intersection.x);
				mQuery.setOutY(intersection.y);
				return false;
			}
		}
		void RayCast::setOut(GridQuery&, const AABB&) { }
	}
}
