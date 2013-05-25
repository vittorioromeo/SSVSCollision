// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

using namespace std;
using namespace ssvu;

namespace ssvsc
{
	GridInfo::GridInfo(Grid& mGrid, Body& mBody) : SpatialInfoBase(mGrid, mBody), grid(mGrid) { bodiesToCheck.reserve(100); }
	GridInfo::~GridInfo() { clear(); }

	void GridInfo::calcEdges()
	{
		const AABB& oldShape(body.getOldShape());
		const AABB& shape(body.getShape());

		oldStartX = startX;
		oldStartY = startY;
		oldEndX = endX;
		oldEndY = endY;

		startX = grid.getIndex(min(oldShape.getLeft(), shape.getLeft()));
		startY = grid.getIndex(min(oldShape.getTop(), shape.getTop()));
		endX = grid.getIndex(max(oldShape.getRight(), shape.getRight()));
		endY = grid.getIndex(max(oldShape.getBottom(), shape.getBottom()));

		if(oldStartX != startX || oldStartY != startY || oldEndX != endX || oldEndY != endY) calcCells();
		else invalid = false;
	}

	void GridInfo::clear()
	{
		for(const auto& cell : cells) cell->del(&body);
		cells.clear(); queries.clear();
	}
	void GridInfo::calcCells()
	{
		clear();

		if(!grid.isIndexValid(startX, startY, endX, endY)) { body.setOutOfBounds(true); return; }
		for(int iY{startY}; iY <= endY; iY++) for(int iX{startX}; iX <= endX; iX++) cells.push_back(&grid.getCell(iX, iY));

		for(const auto& cell : cells)
		{
			cell->add(&body);
			for(const auto& uid : body.getGroupUidsToCheck()) queries.push_back(&cell->getBodies(uid));
		}

		invalid = false;
	}

	void GridInfo::invalidate() { invalid = true; }
	void GridInfo::preUpdate() { if(invalid) calcEdges();  }
	void GridInfo::postUpdate() { }
	const vector<Body*>& GridInfo::getBodiesToCheck()
	{
		bodiesToCheck.clear();
		for(const auto& query : queries) for(const auto& body : *query) if(!contains(bodiesToCheck, body)) bodiesToCheck.push_back(body);
		return bodiesToCheck;
	}
	void GridInfo::destroy() { grid.delSpatialInfo(*this); }
}
