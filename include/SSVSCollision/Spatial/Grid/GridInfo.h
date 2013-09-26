// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRIDINFO
#define SSVSC_SPATIAL_GRIDINFO

#include <vector>
#include "SSVSCollision/Spatial/Grid/Cell.h"

namespace ssvsc
{
	template<typename TS> class GridInfo
	{
		private:
			TS& grid;
			BaseType<TS>& base;
			std::vector<Cell<TS>*> cells;
			int startX{0}, startY{0}, endX{0}, endY{0}; // Edge cell positions
			int oldStartX{-1}, oldStartY{-1}, oldEndX{-1}, oldEndY{-1};
			bool invalid{true};

			inline void calcEdges()
			{
				const AABB& oldShape(this->base.getOldShape());
				const AABB& shape(this->base.getShape());

				oldStartX = startX;
				oldStartY = startY;
				oldEndX = endX;
				oldEndY = endY;

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

				if(!grid.isIndexValid(startX, startY, endX, endY)) { this->base.setOutOfBounds(true); return; }
				for(int iX{startX}; iX <= endX; ++iX)
					for(int iY{startY}; iY <= endY; ++iY)
					{
						auto& c(grid.getCell(iX, iY));
						cells.push_back(&c);
						c.add(&this->base);
					}

				invalid = false;
			}
			inline void clear()
			{
				for(const auto& c : cells) c->del(&this->base);
				cells.clear();
			}

		public:
			int spatialPaint{-1};

			GridInfo(TS& mGrid, BaseType<TS>& mBase) : grid(mGrid), base(mBase) { }

			inline void init() 			{ calcEdges(); calcCells(); }
			inline void invalidate() 	{ invalid = true; }
			inline void preUpdate() 	{ if(invalid) calcEdges(); }
			inline void postUpdate() 	{ }
			inline void destroy() 		{ clear(); }
			inline void handleCollisions(float mFT)
			{
				static int lastPaint{-1};
				++lastPaint;

				for(const auto& c : cells)
					for(const auto& b : c->getBodies())
					{
						if(b->getSpatialInfo().spatialPaint == lastPaint) continue;
						this->base.handleCollision(mFT, b);
						b->getSpatialInfo().spatialPaint = lastPaint;
					}
			}
	};
}

#endif
