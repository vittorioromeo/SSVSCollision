// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/GridQueryTypes.h"
#include "SSVSCollision/Utils/Utils.h"

using namespace std;
using namespace ssvu;
using namespace ssvs::Utils;
using namespace ssvsc::Utils;

namespace ssvsc
{
	namespace GridQueryTypes
	{
		RayCast::RayCast(GridQuery<RayCast, Vec2f>& mQuery, Vec2f mDirection) : Base{mQuery}, cellSize{query.grid.getCellSize()}, direction{getNormalized(mDirection)},
			deltaDist{cellSize / abs(direction.x), cellSize / abs(direction.y)}, increment{direction * static_cast<float>(cellSize)},
			max{Vec2f(query.startIndex * cellSize) - query.startPos}
		{
			next.x = direction.x < 0 ? -1 : 1;
			next.y = direction.y < 0 ? -1 : 1;
			if(direction.x >= 0) max.x += cellSize;
			if(direction.y >= 0) max.y += cellSize;
			max.x /= direction.x;
			max.y /= direction.y;
		}
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
			Segment<float> test1{direction.x > 0 ? mShape.getSegmentLeft<float>() : mShape.getSegmentRight<float>()};
			Segment<float> test2{direction.y > 0 ? mShape.getSegmentTop<float>() : mShape.getSegmentBottom<float>()};

			Vec2f intersection;
			if(isSegmentInsersecting(ray, test1, intersection) || isSegmentInsersecting(ray, test2, intersection))
			{
				query.lastPos = intersection;
				return false;
			}

			return true;
		}


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

			query.lastPos = Vec2f(testX, testY);
			return false;
		}
	}
}
