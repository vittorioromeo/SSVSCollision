// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

using namespace std;
using namespace ssvu;

namespace ssvsc
{
	GridInfo::GridInfo(Grid& mGrid, Base& mBase) : SpatialInfoBase(mGrid, mBase), grid(mGrid) { }
	GridInfo::~GridInfo() { clear(); }

	void GridInfo::calcEdges()
	{
		const AABB& oldShape(base.getOldShape());
		const AABB& shape(base.getShape());

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
		for(const auto& c : cells) c->del(&base);
		cells.clear();
	}
	void GridInfo::calcCells()
	{
		clear();

		if(!grid.isIndexValid(startX, startY, endX, endY)) { base.setOutOfBounds(true); return; }
		for(int iX{startX}; iX <= endX; ++iX) for(int iY{startY}; iY <= endY; ++iY) cells.push_back(&grid.getCell(iX, iY));

		for(const auto& c : cells) c->add(&base);

		invalid = false;
	}

	void GridInfo::invalidate()			{ invalid = true; }
	void GridInfo::preUpdate()			{ if(invalid) calcEdges(); }
	void GridInfo::postUpdate()			{ }
	void GridInfo::destroy()			{ grid.delSpatialInfo(*this); }
	void GridInfo::handleCollisions(float mFrameTime)	{ for(auto& c : cells) for(auto& b : c->getBodies()) base.handleCollision(mFrameTime, b); }
}
