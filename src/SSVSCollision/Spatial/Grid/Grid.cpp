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
		for(int iX{0}; iX < columns; ++iX)
		{
			cells.push_back(vector<Cell*>(rows));
			for(int iY{0}; iY < rows; ++iY)
			{
				int left{iX * cellSize}, right{cellSize + left}, top{iY * cellSize}, bottom{cellSize + top};
				cells[iX][iY] = new Cell{left, right, top, bottom};
			}
		}
	}
	Grid::~Grid()
	{
		memoryManager.clear();
		for(const auto& vector : cells) for(const auto& cell : vector) delete cell;
	}

	SpatialInfoBase& Grid::createSpatialInfo(Body& mBody) { return memoryManager.create(*this, mBody); }
	void Grid::delSpatialInfo(SpatialInfoBase& mSpatialInfo)
	{
		memoryManager.del(&(static_cast<GridInfo&>(mSpatialInfo)));
		memoryManager.cleanUp();
	}
}
