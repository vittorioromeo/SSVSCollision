// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef GRID_H
#define GRID_H

#include <vector>
#include <SSVStart.h>
#include "Grid/GridInfo.h"
#include "Spatial/SpatialBase.h"
#include "Spatial/SpatialInfoBase.h"

namespace ssvsc
{
	class Cell;

	class Grid : public SpatialBase
	{
		private:
			ssvs::Utils::MemoryManager<GridInfo> memoryManager;
			std::vector<std::vector<Cell*>> cells; // owned
			int columns, rows, cellSize, offset;

		public:
			Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~Grid();

			SpatialInfoBase& createSpatialInfo(Body& mBody) override;

			Cell* getCell(int mX, int mY);
			int getIndex(int mValue) const;
			bool isOutside(int mStartX, int mStartY, int mEndX, int mEndY) const;

			void delInfo(GridInfo& mGridInfo);
	};
}

#endif // GRID_H
