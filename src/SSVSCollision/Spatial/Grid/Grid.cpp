// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"

#include "SSVSCollision/Spatial/Grid/Cell.h"
#include "SSVSCollision/Body/Body.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	Grid::Grid(int mColumns, int mRows, int mCellSize, int mOffset) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset}
	{
		for(int iX{0}; iX < columns; iX++)
		{
			cells.push_back(vector<Cell*>(rows));
			for(int iY{0}; iY < rows; iY++)
			{
				int left{iX * cellSize}, right{cellSize + left}, top{iY * cellSize}, bottom{cellSize + top};
				cells[iX][iY] = new Cell{left, right, top, bottom};
			}
		}
	}
	Grid::~Grid()
	{
		memoryManager.clear();
		for(auto& vector : cells) for(auto& cell : vector) delete cell;
	}

	SpatialInfoBase& Grid::createSpatialInfo(Body& mBody) { return memoryManager.create(*this, mBody); }
	void Grid::delSpatialInfo(SpatialInfoBase& mSpatialInfo)
	{
		memoryManager.del(&(static_cast<GridInfo&>(mSpatialInfo)));
		memoryManager.cleanUp();
	}

	int Grid::getCellSize() const { return cellSize; }
	Cell& Grid::getCell(int mX, int mY) { return *(cells[mX + offset][mY + offset]); }
	Cell& Grid::getCell(Vector2i mIndex) { return getCell(mIndex.x, mIndex.y); }
	int Grid::getIndex(int mValue) const { return mValue / cellSize; }
	Vector2i Grid::getIndex(Vector2i mPosition) const { return {getIndex(mPosition.x), getIndex(mPosition.y)}; }
	bool Grid::isOutside(int mStartX, int mStartY, int mEndX, int mEndY) const
	{
		return mStartX < 0 - offset || mEndX >= columns - offset || mStartY < 0 - offset || mEndY >= rows - offset;
	}
	int Grid::getIndexXMin() const { return 0 - offset; }
	int Grid::getIndexYMin() const { return 0 - offset; }
	int Grid::getIndexXMax() const { return columns - offset; }
	int Grid::getIndexYMax() const { return rows - offset; }
	int Grid::getRows() const { return rows; }
	int Grid::getColumns() const { return columns; }
	int Grid::getOffset() const { return offset; }
	bool Grid::isIndexValid(Vector2i mIndex) const { return mIndex.x >= getIndexXMin() && mIndex.x < getIndexXMax() && mIndex.y >= getIndexYMin() && mIndex.y < getIndexYMax(); }
	vector<vector<Cell*>>& Grid::getCells() { return cells; }
}
