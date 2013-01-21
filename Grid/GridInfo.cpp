#include "GridInfo.h"
#include "Grid.h"
#include "Body/Body.h"
#include "Cell.h"

using namespace google;

namespace ssvsc
{
	GridInfo::GridInfo(Grid& mGrid, Body& mBody) : grid(mGrid), body(mBody) { }
	GridInfo::~GridInfo() { clear(); }

	void GridInfo::calcEdges()
	{
		startX = grid.getIndex(body.getLeft());
		startY = grid.getIndex(body.getTop());
		endX = grid.getIndex(body.getRight());
		endY = grid.getIndex(body.getBottom());
	}

	void GridInfo::checkEdges()
	{
		if(grid.getIndex(body.getOldLeft()) != startX) { calcCells(); return; }
		if(grid.getIndex(body.getOldRight()) != endX) { calcCells(); return; }
		if(grid.getIndex(body.getOldTop()) != startY) { calcCells(); return; }
		if(grid.getIndex(body.getOldBottom()) != endY) { calcCells(); return; }
	}

	void GridInfo::clear()
	{
		for(auto& cell : cells) cell->del(&body);
		cells.clear(); queries.clear();
	}
	void GridInfo::calcCells()
	{
		clear();

		if(grid.isOutside(startX, startY, endX, endY)) body.onOutOfBounds();
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

