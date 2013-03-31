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

	GridQuery::GridQuery(Grid& mGrid, Vector2i mStartPos) : grid(mGrid), startPos{Vector2f(mStartPos)}, pos{startPos},
		startIndex{grid.getIndex(mStartPos)}, index{startIndex}, direction(Vector2f(5.f, -2.f)) { }

	Body* GridQuery::nextByDirection(Vector2f mDirection)
	{
		setDirection(mDirection);
		return nextImpl<QueryTraits::RayCast, QueryTraits::Bodies::AllOffset>();
	}
	Body* GridQuery::nextByDirection(Vector2f mDirection, const string& mGroup)
	{
		setDirection(mDirection);
		return nextImpl<QueryTraits::RayCast, QueryTraits::Bodies::GroupedOffset>(mGroup);
	}

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
	Vector2f GridQuery::getSideDist() const		{ return sideDist; }

	// Setters
	void GridQuery::setPos(Vector2f mPos)			{ pos = mPos; }
	void GridQuery::setIndexX(int mIndexX)			{ index.x = mIndexX; }
	void GridQuery::setIndexY(int mIndexY)			{ index.y = mIndexY; }
	void GridQuery::setOutX(float mOutX)			{ out.x = mOutX; }
	void GridQuery::setOutY(float mOutY)			{ out.y = mOutY; }
	void GridQuery::setStepX(int mStepX)			{ step.x = mStepX; }
	void GridQuery::setStepY(int mStepY)			{ step.y = mStepY; }
	void GridQuery::setSideDistX(float mSideDistX)	{ sideDist.x = mSideDistX; }
	void GridQuery::setSideDistY(float mSideDistY)	{ sideDist.y = mSideDistY; }
	void GridQuery::setDirection(Vector2f mDirection)
	{
		direction = getNormalized(mDirection);
		deltaDist = {sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
	}
}
