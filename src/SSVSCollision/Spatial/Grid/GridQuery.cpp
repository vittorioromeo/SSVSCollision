// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Spatial/Grid/GridQuery.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	GridQuery::GridQuery(Grid& mGrid, Vector2i mStart) : grid(mGrid), start{mStart}, current{mStart}, 
		startIndex{grid.getIndex(start.x), grid.getIndex(start.y)}, currentIndex{grid.getIndex(current.x), grid.getIndex(current.y)} { }
	
	void GridQuery::reset()
	{
		current = start;
		currentIndex = {grid.getIndex(current.x), grid.getIndex(current.y)};
		currentBodies.clear();
	}
	
	// Getters
	Vector2i GridQuery::getStart()			{ return start; }
	Vector2i GridQuery::getStartIndex()		{ return startIndex; }	
	Vector2i GridQuery::getCurrent()		{ return current; }
	Vector2i GridQuery::getCurrentIndex()	{ return currentIndex; }
}
