#include "World.h"
#include "Cell.h"
#include "Body.h"

using namespace std;
using namespace ssvs::Utils;

namespace ssvsc
{
	World::World(int mColumns, int mRows, int mCellSize, int mOffset) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset}
	{
		for(int iX{0}; iX < columns; iX++)
			for(int iY{0}; iY < rows; iY++)
			{
				int left{iX * cellSize}; int right{cellSize + left};
				int top{iY * cellSize}; int bottom{cellSize + top};
				cells[{iX, iY}] = new Cell{left, right, top, bottom};
			}
	}
	World::~World() { for(Body* body : bodies) delete body; }

	unordered_set<Cell*> World::calculateCells(Body* mBody)
	{
		int startX{mBody->getLeft() / cellSize + offset};
		int startY{mBody->getTop() / cellSize + offset};
		int endX{mBody->getRight() / cellSize + offset};
		int endY{mBody->getBottom() / cellSize + offset};

		unordered_set<Cell*> result;

		if(startX < 0 || endX >= columns || startY < 0 || endY >= rows)
		{
			mBody->onOutOfBounds();
			return result;
		}

		for(int iY{startY}; iY <= endY; iY++) for(int iX{startX}; iX <= endX; iX++) result.insert(cells[{iX, iY}]);
		return result;
	}

	void World::add(Body* mBody) { bodies.push_back(mBody); }
	void World::del(Body* mBody) { eraseFromVector(bodies, mBody); delete mBody; }
	void World::update(float mFrameTime) { for(Body* body : bodies) body->update(mFrameTime); }
}

