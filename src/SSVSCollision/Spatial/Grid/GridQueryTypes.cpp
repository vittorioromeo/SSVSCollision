// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/GridQueryTypes.h"
#include "SSVSCollision/Spatial/Grid/GridQuery.h"
#include "SSVSCollision/Utils/Utils.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvs::Utils;
using namespace ssvsc::Utils;

namespace ssvsc
{
	namespace GridQueryTypes
	{
		namespace Bodies
		{
			void All::getBodies(vector<Body*>& mBodies, Grid& mGrid, Vector2i& mIndex, int) { mBodies = mGrid.getCell(mIndex).getBodies(); }
			void Grouped::getBodies(vector<Body*>& mBodies, Grid& mGrid, Vector2i& mIndex, int mGroupUid) { mBodies = mGrid.getCell(mIndex).getBodies(); }
		}

		namespace Orthogonal
		{
			Left::Left(GridQuery<Left>& mQuery) : Base{mQuery} { }
			bool Left::isValid() { return query.index.x >= query.grid.getIndexXMin(); }
			void Left::step() { --query.index.x; }
			bool Left::getSorting(const Body* mA, const Body* mB) { return mA->getPosition().x < mB->getPosition().x; }
			bool Left::misses(const AABB& mShape) { return mShape.getLeft() > query.pos.x || query.pos.y < mShape.getTop() || query.pos.y > mShape.getBottom(); }
			void Left::setOut(const AABB& mShape) { query.lastPos = Vector2f(mShape.getRight(), query.pos.y); }

			Right::Right(GridQuery<Right>& mQuery) : Base{mQuery} { }
			bool Right::isValid() { return query.index.x < query.grid.getIndexXMax(); }
			void Right::step() { ++query.index.x; }
			bool Right::getSorting(const Body* mA, const Body* mB) { return mA->getPosition().x > mB->getPosition().x; }
			bool Right::misses(const AABB& mShape) { return mShape.getRight() < query.pos.x || query.pos.y < mShape.getTop() || query.pos.y > mShape.getBottom(); }
			void Right::setOut(const AABB& mShape) { query.lastPos = Vector2f(mShape.getLeft(), query.pos.y); }

			Up::Up(GridQuery<Up>& mQuery) : Base{mQuery} { }
			bool Up::isValid() { return query.index.y >= query.grid.getIndexYMin(); }
			void Up::step() { --query.index.y; }
			bool Up::getSorting(const Body* mA, const Body* mB) { return mA->getPosition().y < mB->getPosition().y; }
			bool Up::misses(const AABB& mShape) { return mShape.getTop() > query.pos.y || query.pos.x < mShape.getLeft() || query.pos.x > mShape.getRight(); }
			void Up::setOut(const AABB& mShape) { query.lastPos = Vector2f(query.pos.x, mShape.getBottom()); }

			Down::Down(GridQuery<Down>& mQuery) : Base{mQuery} { }
			bool Down::isValid() { return query.index.y < query.grid.getIndexYMax(); }
			void Down::step() { ++query.index.y; }
			bool Down::getSorting(const Body* mA, const Body* mB) { return mA->getPosition().y > mB->getPosition().y; }
			bool Down::misses(const AABB& mShape) { return mShape.getBottom() < query.pos.y || query.pos.x < mShape.getLeft() || query.pos.x > mShape.getRight(); }
			void Down::setOut(const AABB& mShape) { query.lastPos = Vector2f(query.pos.x, mShape.getTop()); }
		}


		RayCast::RayCast(GridQuery<RayCast, Vector2f>& mQuery, Vector2f mDirection) : Base{mQuery}, cellSize{query.grid.getCellSize()}, direction{getNormalized(mDirection)},
			deltaDist{cellSize / abs(direction.x), cellSize / abs(direction.y)}, increment{direction * static_cast<float>(cellSize)},
			max{Vector2f(query.startIndex * cellSize) - query.startPos}
		{
			next.x = direction.x < 0 ? -1 : 1;
			next.y = direction.y < 0 ? -1 : 1;
			if(direction.x >= 0) max.x += cellSize;
			if(direction.y >= 0) max.y += cellSize;
			max.x /= direction.x;
			max.y /= direction.y;
		}

		bool RayCast::isValid() { return query.grid.isIndexValid(query.index); }
		void RayCast::step()
		{
			query.lastPos = query.pos;
			query.pos += increment;

			if(max.x < max.y)
			{
				max.x += deltaDist.x;
				query.index.x += next.x;
			}
			else
			{
				max.y += deltaDist.y;
				query.index.y += next.y;
			}
		}
		bool RayCast::getSorting(const Body* mA, const Body* mB)
		{
			const auto& aPos(mA->getPosition());
			const auto& bPos(mB->getPosition());
			return pow((aPos.x - query.startPos.x), 2) + pow((aPos.y - query.startPos.y), 2) > pow((bPos.x - query.startPos.x), 2) + pow((bPos.y - query.startPos.y), 2);
		}
		bool RayCast::misses(const AABB& mShape)
		{
			Segment<float> ray{query.startPos, query.pos};
			Segment<float> test1{direction.x > 0 ? mShape.getLeftSegment<float>() : mShape.getRightSegment<float>()};
			Segment<float> test2{direction.y > 0 ? mShape.getTopSegment<float>() : mShape.getBottomSegment<float>()};

			Vector2f intersection;
			if(isSegmentInsersecting(ray, test1, intersection) || isSegmentInsersecting(ray, test2, intersection))
			{
				query.lastPos = intersection;
				return false;
			}

			return true;
		}
		void RayCast::setOut(const AABB&) { }


		Distance::Distance(GridQuery<Distance, int>& mQuery, int mDistance) : Base{mQuery}, cellSize{query.grid.getCellSize()}, distance{mDistance},
			cellRadius{distance / cellSize}
		{
			for(int iRadius{0}; iRadius < cellRadius + 1; ++iRadius)
			{
				for(int iY{-iRadius}; iY <= iRadius; ++iY)
				{
					offsets.push({iRadius, iY});
					if(-iRadius != iRadius) offsets.push({-iRadius, iY});
				}

				for(int iX{-iRadius + 1}; iX <= iRadius -1; ++iX)
				{
					offsets.push({iX, iRadius});
					offsets.push({iX, -iRadius});
				}
			}
		}

		bool Distance::isValid() { return !offsets.empty() && query.grid.isIndexValid(query.index); }
		void Distance::step()
		{
			query.lastPos = query.pos;
			query.index = query.startIndex + offsets.front();

			if(!offsets.empty()) offsets.pop();
		}
		bool Distance::getSorting(const Body* mA, const Body* mB)
		{
			const auto& aPos(mA->getPosition());
			const auto& bPos(mB->getPosition());
			return pow((aPos.x - query.startPos.x), 2) + pow((aPos.y - query.startPos.y), 2) > pow((bPos.x - query.startPos.x), 2) + pow((bPos.y - query.startPos.y), 2);
		}
		bool Distance::misses(const AABB& mShape)
		{
			int testX{query.startPos.x < mShape.getX() ? mShape.getLeft() : mShape.getRight()};
			int testY{query.startPos.y < mShape.getY() ? mShape.getTop() : mShape.getBottom()};

			if(pow((testX - query.startPos.x), 2) + pow((testY - query.startPos.y), 2) > pow(distance, 2)) return true;

			query.lastPos = Vector2f(testX, testY);
			return false;
		}
		void Distance::setOut(const AABB&) { }
	}
}
