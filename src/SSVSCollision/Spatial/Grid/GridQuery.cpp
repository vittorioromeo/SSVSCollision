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

	Body* GridQuery::next(Vector2f mDirection) { return nextHelper<QueryTraits::Bodies::All>(mDirection); }
	Body* GridQuery::next(Vector2f mDirection, const string& mGroup) { return nextHelper<QueryTraits::Bodies::Grouped>(mDirection, mGroup); }

	void GridQuery::reset()
	{
		pos = startPos;
		index = startIndex;
		bodies.clear();
		visitedIndexes.clear();
	}

	// Getters
	Grid& GridQuery::getGrid() 			{ return grid; }
	const Vector2f& GridQuery::getStartPos() 		{ return startPos; }
	const Vector2i& GridQuery::getStartIndex() 	{ return startIndex; }
	const Vector2f& GridQuery::getPos() 			{ return pos; }
	const Vector2f& GridQuery::getOut() 			{ return out; }
	const Vector2i& GridQuery::getIndex()		{ return index; }
	const Vector2f& GridQuery::getDirection() 	{ return direction; }
	const Vector2i& GridQuery::getStep() 			{ return step; }
	const Vector2f& GridQuery::getDeltaDist() 	{ return deltaDist; }
	const vector<Vector2i>& GridQuery::getVisitedIndexes()  { return visitedIndexes; }
	const Vector2f& GridQuery::getMax() 		{ return max; }
	const Vector2f& GridQuery::getIncrement() 	{ return increment; }
	
	// Setters
	void GridQuery::setBodies(vector<Body*> mBodies)	{ bodies = mBodies; }
	void GridQuery::setPos(Vector2f mPos)				{ pos = mPos; }
	void GridQuery::setIndexX(int mIndexX)				{ visitedIndexes.push_back(index); index.x = mIndexX; }
	void GridQuery::setIndexY(int mIndexY)				{ visitedIndexes.push_back(index); index.y = mIndexY; }
	void GridQuery::setOut(Vector2f mOut)				{ out = mOut; }
	void GridQuery::setOutX(float mOutX)				{ out.x = mOutX; }
	void GridQuery::setOutY(float mOutY)				{ out.y = mOutY; }
	void GridQuery::setStepX(int mStepX)				{ step.x = mStepX; }
	void GridQuery::setStepY(int mStepY)				{ step.y = mStepY; }
	void GridQuery::setMaxX(float mMaxX)				{ max.x = mMaxX; }
	void GridQuery::setMaxY(float mMaxY)				{ max.y = mMaxY; }
	void GridQuery::setDirection(Vector2f mDirection)
	{
		const int cellSize{grid.getCellSize()};
		direction = getNormalized(mDirection);
		deltaDist = {cellSize / abs(direction.x), cellSize / abs(direction.y)};
		increment = direction * static_cast<float>(cellSize);
		
		step.x = direction.x < 0 ? -1 : 1;
        step.y = direction.y < 0 ? -1 : 1;
		
		max = Vector2f(startIndex * cellSize) - startPos;
		if(direction.x >= 0) max.x += cellSize;
		if(direction.y >= 0) max.y += cellSize;
		max.x /= direction.x;
		max.y /= direction.y;		
	}
}
