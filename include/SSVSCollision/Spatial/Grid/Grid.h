// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_GRID
#define SSVSC_GRID

#include <vector>
#include <SFML/System.hpp>
#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/SpatialBase.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"

namespace ssvsc
{
	class Cell;
	class GridQuery;
	
	class Grid : public SpatialBase
	{
		private:
			ssvu::MemoryManager<GridInfo> memoryManager;
			std::vector<std::vector<Cell*>> cells; // owned
			int columns, rows, cellSize, offset;

		public:
			Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~Grid();

			SpatialInfoBase& createSpatialInfo(Body& mBody) override;
			void delSpatialInfo(SpatialInfoBase& mSpatialInfo) override;
			
			int getCellSize() const;
			Cell& getCell(int mX, int mY);
			Cell& getCell(sf::Vector2i mIndex);
			int getIndex(int mValue) const;
			sf::Vector2i getIndex(sf::Vector2i mPosition) const;
			bool isOutside(int mStartX, int mStartY, int mEndX, int mEndY) const;
			int getXMinIndex() const;
			int getYMinIndex() const;
			int getXMaxIndex() const;
			int getYMaxIndex() const;
			
			GridQuery getQuery(sf::Vector2i mPoint);
	};
}

#endif
