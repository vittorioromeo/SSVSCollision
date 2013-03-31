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

namespace ssvsc
{
	Vector2i lineIntersection(Vector2i mA1, Vector2i mA2, Vector2i mB1, Vector2i mB2)
	{
		float d = (mA1.x - mA2.x) * (mB1.y - mB2.y) - (mA1.y - mA2.y) * (mB1.x - mB2.x);
		// If d is zero, there is no intersection
		if (d == 0) return Vector2i(0, 0);
		 
		// Get the x and y
		float pre = (mA1.x*mA2.y - mA1.y*mA2.x), post = (mB1.x*mB2.y - mB1.y*mB2.x);
		float x = ( pre * (mB1.x - mB2.x) - (mA1.x - mA2.x) * post ) / d;
		float y = ( pre * (mB1.y - mB2.y) - (mA1.y - mA2.y) * post ) / d;
		 
		// Check if the x and y coordinates are within both lines
		if ( x < min(mA1.x, mA2.x) || x > max(mA1.x, mA2.x) ||
		x < min(mB1.x, mB2.x) || x > max(mB1.x, mB2.x) ) return Vector2i(0, 0);
		if ( y < min(mA1.y, mA2.y) || y > max(mA1.y, mA2.y) ||
		y < min(mB1.y, mB2.y) || y > max(mB1.y, mB2.y) ) return Vector2i(0, 0);
		 
		return Vector2i(x, y);
	}
	
	GridQuery::GridQuery(Grid& mGrid, Vector2i mStartPos) : grid(mGrid), startPos{mStartPos}, pos{startPos}, 
		startIndex{grid.getIndex(startPos)}, index{startIndex}, direction(Vector2f(5.f, -2.f)) { }
	
	Body*GridQuery::nextByDirection(Vector2f mDirection)
	{
		direction = ssvs::Utils::getNormalized(mDirection);
		deltaDist = {(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
		return nextImpl<QueryTraits::RayCast, QueryTraits::Bodies::All>();
	}	
	Body*GridQuery::nextByDirection(Vector2f mDirection, const string& mGroup)
	{
		direction = ssvs::Utils::getNormalized(mDirection);
		deltaDist = {(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
		return nextImpl<QueryTraits::RayCast, QueryTraits::Bodies::Grouped>(mGroup);
	}
	
	void GridQuery::reset()
	{
		pos = startPos;
		index = startIndex;
		deltaDist = {(float)sqrt(1 + (direction.y * direction.y) / (direction.x * direction.x)), (float)sqrt(1 + (direction.x * direction.x) / (direction.y * direction.y))};
		bodies.clear();
	}

	// Getters
	Vector2i GridQuery::getStartPos()	{ return startPos; }
	Vector2i GridQuery::getStartIndex()	{ return startIndex; }	
	Vector2i GridQuery::getPos()		{ return pos; }
	Vector2i GridQuery::getIndex()		{ return index; }
}
