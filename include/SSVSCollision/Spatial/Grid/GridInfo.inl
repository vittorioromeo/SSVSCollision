// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRIDINFO_INL
#define SSVSC_SPATIAL_GRIDINFO_INL

namespace ssvsc
{
	inline GridInfo::GridInfo(Grid& mGrid, Base& mBase) : SpatialInfoBase(mGrid, mBase), grid(mGrid) { }
	inline void GridInfo::calcEdges()
	{
		const AABB& oldShape(base.getOldShape());
		const AABB& shape(base.getShape());

		oldStartX = startX;
		oldStartY = startY;
		oldEndX = endX;
		oldEndY = endY;

		startX = grid.getIndex(std::min(oldShape.getLeft(), shape.getLeft()));
		startY = grid.getIndex(std::min(oldShape.getTop(), shape.getTop()));
		endX = grid.getIndex(std::max(oldShape.getRight(), shape.getRight()));
		endY = grid.getIndex(std::max(oldShape.getBottom(), shape.getBottom()));

		if(oldStartX != startX || oldStartY != startY || oldEndX != endX || oldEndY != endY) calcCells();
		else invalid = false;
	}
	inline void GridInfo::calcCells()
	{
		clear();

		if(!grid.isIndexValid(startX, startY, endX, endY)) { base.setOutOfBounds(true); return; }
		for(int iX{startX}; iX <= endX; ++iX)
			for(int iY{startY}; iY <= endY; ++iY)
			{
				auto& c(grid.getCell(iX, iY));
				cells.push_back(&c);
				c.add(&base);
			}

		invalid = false;
	}
}

#endif
