// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "GridInfo.h"
#include "Grid.h"
#include "Body/Body.h"
#include "Cell.h"

using namespace google;

namespace ssvsc
{
	GridInfo::GridInfo(Grid& mGrid, Body& mBody) : SpatialInfoBase(mGrid, mBody), grid(mGrid) { }
	GridInfo::~GridInfo() { clear(); }

	void GridInfo::calcEdges()
	{
		AABB& shape(body.getShape());
		startX = grid.getIndex(shape.getLeft());
		startY = grid.getIndex(shape.getTop());
		endX = grid.getIndex(shape.getRight());
		endY = grid.getIndex(shape.getBottom());
	}

	void GridInfo::checkEdges()
	{
		AABB& oldShape(body.getOldShape());
		if(grid.getIndex(oldShape.getLeft()) != startX) { calcCells(); return; }
		if(grid.getIndex(oldShape.getRight()) != endX) { calcCells(); return; }
		if(grid.getIndex(oldShape.getTop()) != startY) { calcCells(); return; }
		if(grid.getIndex(oldShape.getBottom()) != endY) { calcCells(); return; }
	}

	void GridInfo::clear()
	{
		for(auto& cell : cells) cell->del(&body);
		cells.clear(); queries.clear();
	}
	void GridInfo::calcCells()
	{
		clear();

		if(grid.isOutside(startX, startY, endX, endY)) { body.setOutOfBounds(true); return; }
		for(int iY{startY}; iY <= endY; iY++) for(int iX{startX}; iX <= endX; iX++) cells.push_back(grid.getCell(iX, iY));

		for(auto& cell : cells)
		{
			cell->add(&body);
			for(auto& group : body.getGroupsToCheck()) queries.push_back(cell->getQuery(group));
		}

		invalid = false;
	}

	void GridInfo::invalidate() { invalid = true; }
	void GridInfo::preUpdate() { if(invalid) { calcEdges(); calcCells(); } }
	void GridInfo::postUpdate() { calcEdges(); checkEdges(); }
	dense_hash_set<Body*> GridInfo::getBodiesToCheck()
	{
		dense_hash_set<Body*> result; result.set_empty_key(nullptr);
		for(auto& query : queries) for(auto& body : *query) result.insert(body);
		return result;
	}
}

