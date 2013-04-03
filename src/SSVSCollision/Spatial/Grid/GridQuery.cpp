// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Spatial/Grid/GridQuery.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvs::Utils;

namespace ssvsc
{
	GridQuery::GridQuery(Grid& mGrid, Vector2i mStartPos) : grid(mGrid), startPos{Vector2f(mStartPos)}, pos{startPos},
		startIndex{grid.getIndex(mStartPos)}, index{startIndex}, direction{0, 0} { }

	Body* GridQuery::next(Vector2f mDirection) { return nextHelper<QueryTraits::Bodies::All, QueryTraits::Bodies::AllOffset>(mDirection); }
	Body* GridQuery::next(Vector2f mDirection, const string& mGroup) { return nextHelper<QueryTraits::Bodies::Grouped, QueryTraits::Bodies::GroupedOffset>(mDirection, mGroup); }

	void GridQuery::reset()
	{
		pos = startPos;
		index = startIndex;
		bodies.clear();
	}

	// Getters
	Grid& GridQuery::getGrid() const			{ return grid; }
	Vector2f GridQuery::getStartPos() const		{ return startPos; }
	Vector2i GridQuery::getStartIndex() const	{ return startIndex; }
	Vector2f GridQuery::getPos() const			{ return pos; }
	Vector2f GridQuery::getOut() const			{ return out; }
	Vector2i GridQuery::getIndex() const		{ return index; }
	Vector2f GridQuery::getDirection() const	{ return direction; }
	Vector2i GridQuery::getStep() const			{ return step; }
	Vector2f GridQuery::getDeltaDist() const	{ return deltaDist; }
	vector<Vector2i> GridQuery::getVisitedIndexes() const { return visitedIndexes; }
	Vector2f& GridQuery::getMax() 		{ return max; }
	
	// Setters
	void GridQuery::setBodies(vector<Body*> mBodies)	{ bodies = mBodies; }
	void GridQuery::setPos(Vector2f mPos)				{ pos = mPos; }
	void GridQuery::setIndexX(int mIndexX)				{ visitedIndexes.push_back(index); index.x = mIndexX; }
	void GridQuery::setIndexY(int mIndexY)				{ visitedIndexes.push_back(index); index.y = mIndexY; }
	void GridQuery::setOutX(float mOutX)				{ out.x = mOutX; }
	void GridQuery::setOutY(float mOutY)				{ out.y = mOutY; }
	void GridQuery::setStepX(int mStepX)				{ step.x = mStepX; }
	void GridQuery::setStepY(int mStepY)				{ step.y = mStepY; }
	void GridQuery::setDirection(Vector2f mDirection)
	{
		const int cellSize{grid.getCellSize()};
		direction = getNormalized(mDirection);
		deltaDist = {cellSize / abs(direction.x), cellSize / abs(direction.y)};
		
		step.x = direction.x < 0 ? -1 : 1;
        step.y = direction.y < 0 ? -1 : 1;
		
		max = Vector2f(startIndex * cellSize) - startPos;
		if(direction.x >= 0) max.x += cellSize;
		if(direction.y >= 0) max.y += cellSize;
		max.x /= direction.x;
		max.y /= direction.y;		
	}
}
