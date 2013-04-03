// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/GridQueryTraits.h"
#include "SSVSCollision/Spatial/Grid/GridQuery.h"
#include "SSVSCollision/Utils/Utils.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvsc::Utils;

namespace ssvsc
{
	namespace QueryTraits
	{
		namespace Bodies
		{
			void All::getBodies(GridQuery& mQuery, const string&) { mQuery.setBodies(mQuery.getGrid().getCell(mQuery.getIndex()).getBodies()); }
			void Grouped::getBodies(GridQuery& mQuery, const string& mGroup) { mQuery.setBodies(mQuery.getGrid().getCell(mQuery.getIndex()).getBodies(mGroup)); }
		}

		namespace Orthogonal
		{
			bool Left::isValid(GridQuery& mQuery) { return mQuery.getIndex().x >= mQuery.getGrid().getIndexXMin(); }
			void Left::step(GridQuery& mQuery) { mQuery.setIndexX(mQuery.getIndex().x - 1); }
			bool Left::getSorting(GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().x < mB->getPosition().x; }
			bool Left::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getLeft() > mQuery.getPos().x || mQuery.getPos().y < mShape.getTop() || mQuery.getPos().y > mShape.getBottom(); }
			void Left::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mShape.getRight()); mQuery.setOutY(mQuery.getPos().y); }

			bool Right::isValid(GridQuery& mQuery) { return mQuery.getIndex().x < mQuery.getGrid().getIndexXMax(); }
			void Right::step(GridQuery& mQuery) { mQuery.setIndexX(mQuery.getIndex().x + 1); }
			bool Right::getSorting(GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().x > mB->getPosition().x; }
			bool Right::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getRight() < mQuery.getPos().x || mQuery.getPos().y < mShape.getTop() || mQuery.getPos().y > mShape.getBottom(); }
			void Right::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mShape.getLeft()); mQuery.setOutY(mQuery.getPos().y); }

			bool Up::isValid(GridQuery& mQuery) { return mQuery.getIndex().y >= mQuery.getGrid().getIndexYMin(); }
			void Up::step(GridQuery& mQuery) { mQuery.setIndexY(mQuery.getIndex().y - 1); }
			bool Up::getSorting(GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().y < mB->getPosition().y; }
			bool Up::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getTop() > mQuery.getPos().y || mQuery.getPos().x < mShape.getLeft() || mQuery.getPos().x > mShape.getRight(); }
			void Up::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mQuery.getPos().x); mQuery.setOutY(mShape.getBottom()); }

			bool Down::isValid(GridQuery& mQuery) { return mQuery.getIndex().y < mQuery.getGrid().getIndexYMax(); }
			void Down::step(GridQuery& mQuery) { mQuery.setIndexY(mQuery.getIndex().y + 1); }
			bool Down::getSorting(GridQuery&, const Body* mA, const Body* mB) { return mA->getPosition().y > mB->getPosition().y; }
			bool Down::misses(GridQuery& mQuery, const AABB& mShape) { return mShape.getBottom() < mQuery.getPos().y || mQuery.getPos().x < mShape.getLeft() || mQuery.getPos().x > mShape.getRight(); }
			void Down::setOut(GridQuery& mQuery, const AABB& mShape) { mQuery.setOutX(mQuery.getPos().x); mQuery.setOutY(mShape.getTop()); }
		}

		bool RayCast::isValid(GridQuery& mQuery) { return mQuery.getGrid().isIndexValid(mQuery.getIndex()); }
		void RayCast::step(GridQuery& mQuery)
		{
			const auto& index(mQuery.getIndex());
			const auto& step(mQuery.getStep());
			const auto& deltaDist(mQuery.getDeltaDist());
			const auto& pos(mQuery.getPos());
			const auto& max(mQuery.getMax());
		
			mQuery.setOut(pos); 
			mQuery.setPos(pos + mQuery.getIncrement());

			if (max.x < max.y)
			{
				mQuery.setMaxX(max.x + deltaDist.x);
				mQuery.setIndexX(index.x + step.x);
			}
			else
			{
				mQuery.setMaxY(max.y + deltaDist.y);
				mQuery.setIndexY(index.y + step.y);
			}
		}
		bool RayCast::getSorting(GridQuery& mQuery, const Body* mA, const Body* mB)
		{
			const auto& startPos(mQuery.getStartPos());
			const auto& aPos(mA->getPosition());
			const auto& bPos(mB->getPosition());
			return sqrt(pow((aPos.x - startPos.x), 2) + pow((aPos.y - startPos.y), 2)) > sqrt(pow((bPos.x - startPos.x), 2) + pow((bPos.y - startPos.y), 2));
		}
		bool RayCast::misses(GridQuery& mQuery, const AABB& mShape)
		{
			const auto& direction(mQuery.getDirection());
			Segment<float> ray{mQuery.getStartPos(), mQuery.getPos()};
			vector<Segment<float>> lines;

			if(direction.x > 0) lines.push_back(mShape.getLeftSegment<float>());
			else lines.push_back(mShape.getRightSegment<float>());

			if(direction.y > 0) lines.push_back(mShape.getTopSegment<float>());
			else lines.push_back(mShape.getBottomSegment<float>());

			bool intersects{false};
			Vector2f intersection;

			while(!intersects && !lines.empty())
			{
				auto currentLine(lines.back());
				lines.pop_back();
				intersects = isSegmentInsersecting(ray, currentLine, intersection);
			}

			if(intersects) { mQuery.setOut(intersection); return false; }

			return true;
		}
		void RayCast::setOut(GridQuery&, const AABB&) { }
	}
}
