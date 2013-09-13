// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRIDINFO
#define SSVSC_SPATIAL_GRIDINFO

#include <vector>
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"

namespace ssvsc
{
	template<typename TGrid> class GridInfo : public SpatialInfoBase
	{
		private:
			TGrid& grid;
			std::vector<Cell*> cells;
			int startX{0}, startY{0}, endX{0}, endY{0}; // Edge cell positions
			int oldStartX{-1}, oldStartY{-1}, oldEndX{-1}, oldEndY{-1};
			bool invalid{true};

			inline void calcEdges()
			{
				const AABB& oldShape(base.getOldShape());
				const AABB& shape(base.getShape());

				oldStartX = startX;
				oldStartY = startY;
				oldEndX = endX;
				oldEndY = endY;

				/*startX = grid.getIndex(std::min(oldShape.getLeft(), shape.getLeft()));
				startY = grid.getIndex(std::min(oldShape.getTop(), shape.getTop()));
				endX = grid.getIndex(std::max(oldShape.getRight(), shape.getRight()));
				endY = grid.getIndex(std::max(oldShape.getBottom(), shape.getBottom()));*/

				startX = grid.getIndex(shape.getLeft());
				startY = grid.getIndex(shape.getTop());
				endX = grid.getIndex(shape.getRight());
				endY = grid.getIndex(shape.getBottom());

				if(oldStartX != startX || oldStartY != startY || oldEndX != endX || oldEndY != endY) calcCells();
				else invalid = false;
			}
			inline void calcCells()
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
			inline void clear()
			{
				for(const auto& c : cells) c->del(&base);
				cells.clear();
			}

		public:
			GridInfo(TGrid& mGrid, Base& mBase) : SpatialInfoBase(mGrid, mBase), grid(mGrid) { }

			inline void init() override			{ calcEdges(); calcCells(); }
			inline void invalidate() override	{ invalid = true; }
			inline void preUpdate() override	{ if(invalid) calcEdges(); }
			inline void postUpdate() override	{ }
			inline void destroy() override		{ clear(); SpatialInfoBase::destroy(); }
			inline void handleCollisions(float mFrameTime) override
			{
				static int lastPaint{-1};
				++lastPaint;

				for(const auto& c : cells)
					for(const auto& b : c->getBodies())
					{
						if(b->getSpatialPaint() == lastPaint) continue;
						base.handleCollision(mFrameTime, b);
						b->setSpatialPaint(lastPaint);
					}
			}
	};
}

#endif
