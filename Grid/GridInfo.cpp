#include "GridInfo.h"
#include "Grid.h"
#include "Body/Body.h"
#include "Cell.h"

using namespace google;

namespace ssvsc
{
	GridInfo::GridInfo(Grid& mGrid, Body& mBody) : grid(mGrid), body(mBody) { }
	GridInfo::~GridInfo() { clearCells(); }

	void GridInfo::recalcEdges()
	{
		startX = grid.getIndex(body.getLeft());
		startY = grid.getIndex(body.getTop());
		endX = grid.getIndex(body.getRight());
		endY = grid.getIndex(body.getBottom());
	}

	void GridInfo::checkOldEdges()
	{
		if(grid.getIndex(body.getOldLeft()) != startX) { recalcCells(); return; }
		if(grid.getIndex(body.getOldTop()) != startY) { recalcCells(); return; }
		if(grid.getIndex(body.getOldRight()) != endX) { recalcCells(); return; }
		if(grid.getIndex(body.getOldBottom()) != endY) { recalcCells(); return; }
	}

	void GridInfo::clearCells()
	{
		for(auto& cell : cells) cell->del(&body);
		cells.clear(); queries.clear();
	}
	void GridInfo::recalcCells()
	{
		clearCells();

		if(grid.isOutside(startX, startY, endX, endY)) body.onOutOfBounds();
		for(int iY{startY}; iY <= endY; iY++) for(int iX{startX}; iX <= endX; iX++) cells.push_back(grid.getCell(iX, iY));

		for(auto& cell : cells)
		{
			cell->add(&body);
			for(auto& group : body.getGroupsToCheck()) queries.push_back(cell->getQuery(group));
		}

		mustRecalculate = false;
	}

	void GridInfo::groupsChanged() { mustRecalculate = true; }
	void GridInfo::beforeUpdate() { if(mustRecalculate) { recalcEdges(); recalcCells(); } }
	void GridInfo::afterUpdate() { recalcEdges(); checkOldEdges(); }
	dense_hash_set<Body*> GridInfo::getBodiesToCheck()
	{
		dense_hash_set<Body*> result;
		result.set_empty_key(nullptr);
		for(auto& query : queries) for(auto& body : *query) result.insert(body);
		return result;
	}
}

