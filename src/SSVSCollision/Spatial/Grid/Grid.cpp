// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/Resolver/ResolverBase.h"

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
				cells[iX][iY] = new Cell{AABB{left, right, top, bottom}};
			}
		}
	}
	Grid::~Grid()
	{
		memoryManager.clear();
		for(int iX{0}; iX < columns; ++iX) for(int iY{0}; iY < rows; ++iY) delete cells[iX][iY];
	}

	SpatialInfoBase& Grid::createSpatialInfo(Base& mBase) { return memoryManager.create(*this, mBase); }
	void Grid::delSpatialInfo(SpatialInfoBase& mSpatialInfo)
	{
		memoryManager.del(&(static_cast<GridInfo&>(mSpatialInfo)));
		memoryManager.cleanUp();
	}

	void Grid::handleCollisions(float mFrameTime)
	{
		for(int iX{0}; iX < columns; ++iX)
			for(int iY{0}; iY < rows; ++iY)
			{
				const auto& bodies(cells[iX][iY]->getBodies());
				const auto& size(bodies.size());

				for(unsigned int i{0}; i < size; i++)
					for(unsigned int j{i + 1}; j < size; j++)
					{
						Body& a(*bodies[i]);
						Body& b(*bodies[j]);

						if(a.mustCheckAgainst(b) || b.mustCheckAgainst(a))
						{
							const auto& aShape(a.getShape());
							const auto& bShape(b.getShape());

							if(aShape.isOverlapping(bShape))
							{
								auto intersection(Utils::getMinIntersection(aShape, bShape));

								a.onDetection({b, mFrameTime, b.getUserData(), intersection});
								b.onDetection({a, mFrameTime, a.getUserData(), -intersection});

								if(a.mustResolveAgainst(b)) a.addToResolveAgainst(b);
								if(b.mustResolveAgainst(a)) b.addToResolveAgainst(a);
							}
						}
					}
			}
	}
}
