#include "World.h"
#include "Cell.h"
#include "Body.h"

using namespace std;
using namespace ssvs::Utils;

namespace ssvsc
{
	World::World(int mColumns, int mRows, int mCellSize, int mOffset) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset}
	{
		bodiesToDel.set_empty_key(nullptr);

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
	World::~World()
	{
		for(auto& body : bodies) delete body;
		for(auto& vector : cells) for(auto& cell : vector) delete cell;
	}

	void World::add(Body* mBody) { bodies.push_back(mBody); }
	void World::del(Body* mBody) { bodiesToDel.insert(mBody); }
	void World::update(float mFrameTime)
	{
		// Delete body ptrs, removing them from their own cells and from the bodies vector
		for(auto& body : bodiesToDel) { body->clearCells(); eraseFromVector(bodies, body); delete body; }
		bodiesToDel.clear();

		for(auto& body : bodies) body->update(mFrameTime);
	}

	Cell* World::getCell(int mX, int mY) { return cells[mX + offset][mY + offset]; }
	int World::getIndex(int mValue) { return mValue / cellSize; }
	bool World::isOutside(int mStartX, int mStartY, int mEndX, int mEndY) { return mStartX < 0 || mEndX >= columns || mStartY < 0 || mEndY >= rows; }
}

