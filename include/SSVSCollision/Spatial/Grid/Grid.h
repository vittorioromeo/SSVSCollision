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

			SpatialInfoBase& createSpatialInfo(Body& mBody) override;
			void delSpatialInfo(SpatialInfoBase& mSpatialInfo) override;

			inline int getIndexXMin() const			{ return 0 - offset; }
			inline int getIndexYMin() const			{ return 0 - offset; }
			inline int getIndexXMax() const			{ return columns - offset; }
			inline int getIndexYMax() const			{ return rows - offset; }
			inline int getRows() const				{ return rows; }
			inline int getColumns() const			{ return columns; }
			inline int getOffset() const			{ return offset; }
			inline int getCellSize() const			{ return cellSize; }

			inline int getIndex(int mValue) const						{ return mValue / cellSize; }
			inline sf::Vector2i getIndex(sf::Vector2i mPosition) const	{ return {getIndex(mPosition.x), getIndex(mPosition.y)}; }

			inline Cell& getCell(int mX, int mY)		{ return *(cells[mX + offset][mY + offset]); }
			inline Cell& getCell(sf::Vector2i mIndex)	{ return getCell(mIndex.x, mIndex.y); }

			inline const std::vector<std::vector<Cell*>>& getCells() { return cells; }
			inline bool isIndexValid(sf::Vector2i mIndex) const { return mIndex.x >= getIndexXMin() && mIndex.x < getIndexXMax() && mIndex.y >= getIndexYMin() && mIndex.y < getIndexYMax(); }
			inline bool isIndexValid(int mStartX, int mStartY, int mEndX, int mEndY) const { return mStartX >= getIndexXMin() && mEndX < getIndexXMax() && mStartY >= getIndexYMin() && mEndY < getIndexYMax(); }

			template<typename T, typename... TArgs> GridQuery<T, TArgs...> getQuery(sf::Vector2i mPoint, TArgs... mArgs) { return {*this, mPoint, mArgs...}; }
	};
}

#endif
