// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRIDINFO
#define SSVSC_SPATIAL_GRIDINFO

#include <vector>
#include "SSVSCollision/Spatial/Grid/Cell.h"

namespace ssvsc
{
	template<typename TW> class GridInfo
	{
		public:
			using SpatialType = typename TW::SpatialType;
			using BaseType = Base<TW>;
			using CellType = Cell<TW>;

		private:
			SpatialType& grid;
			BaseType& base;
			std::vector<CellType*> cells;
			int startX{0}, startY{0}, endX{0}, endY{0}, oldStartX{-1}, oldStartY{-1}, oldEndX{-1}, oldEndY{-1}, spatialPaint{-1};
			bool invalid{true};

			inline void calcEdges()
			{
				const AABB& oldShape(base.getOldShape());
				const AABB& shape(base.getShape());

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
			inline GridInfo(SpatialType& mGrid, BaseType& mBase) noexcept : grid(mGrid), base(mBase) { }

			inline void init()								{ calcEdges(); calcCells(); }
			inline void invalidate() noexcept				{ invalid = true; }
			inline void preUpdate()							{ if(invalid) calcEdges(); }
			inline void postUpdate() const noexcept			{ }
			inline void destroy()							{ clear(); }
			inline void handleCollisions(float mFT)
			{
				static int lastPaint{-1};
				++lastPaint;

				for(const auto& c : cells)
					for(const auto& b : c->getBodies())
					{
						if(b->getSpatialInfo().spatialPaint == lastPaint) continue;
						base.handleCollision(mFT, b);
						b->getSpatialInfo().spatialPaint = lastPaint;
					}
			}
	};
}

#endif
