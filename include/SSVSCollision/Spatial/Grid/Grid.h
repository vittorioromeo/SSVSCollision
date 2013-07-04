// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID
#define SSVSC_SPATIAL_GRID

#include <vector>
#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/SpatialBase.h"
#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	class Cell;
	class SpatialInfoBase;
	class ResolverBase;
	template<typename T, typename... TArgs> class GridQuery;

	class Grid : public SpatialBase
	{
		private:
			ssvu::MemoryManager<GridInfo> memoryManager;
			std::vector<std::vector<Cell*>> cells; // owned
			int columns, rows, cellSize, offset;

		public:
			Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~Grid();

			SpatialInfoBase& createSpatialInfo(Base& mBase) override;
			void delSpatialInfo(SpatialInfoBase& mSpatialInfo) override;

			inline int getIndexXMin() const	{ return 0 - offset; }
			inline int getIndexYMin() const	{ return 0 - offset; }
			inline int getIndexXMax() const	{ return columns - offset; }
			inline int getIndexYMax() const	{ return rows - offset; }
			inline int getRows() const		{ return rows; }
			inline int getColumns() const	{ return columns; }
			inline int getOffset() const	{ return offset; }
			inline int getCellSize() const	{ return cellSize; }

			inline int getIndex(int mValue) const			{ return mValue / cellSize; }
			inline Vec2i getIndex(Vec2i mPosition) const	{ return {getIndex(mPosition.x), getIndex(mPosition.y)}; }

			inline Cell& getCell(int mX, int mY)	{ return *cells[mX + offset][mY + offset]; }
			inline Cell& getCell(Vec2i mIndex)		{ return getCell(mIndex.x, mIndex.y); }

			inline const std::vector<std::vector<Cell*>>& getCells() { return cells; }
			inline bool isIndexValid(Vec2i mIndex) const									{ return mIndex.x >= getIndexXMin() && mIndex.x < getIndexXMax() && mIndex.y >= getIndexYMin() && mIndex.y < getIndexYMax(); }
			inline bool isIndexValid(int mStartX, int mStartY, int mEndX, int mEndY) const	{ return mStartX >= getIndexXMin() && mEndX < getIndexXMax() && mStartY >= getIndexYMin() && mEndY < getIndexYMax(); }

			template<typename T, typename... TArgs> GridQuery<T, TArgs...> getQuery(Vec2i mPoint, TArgs... mArgs) { return {*this, mPoint, mArgs...}; }
	};
}

#endif
