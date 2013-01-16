#include "World.h"
#include "Cell.h"
#include "Body.h"

using namespace std;

namespace ssvsc
{
	World::World(int mColumns, int mRows, int mCellSize, int mOffset) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset}
	{
		for(int iX{0}; iX < columns; iX++)
			for(int iY{0}; iY < rows; iY++)
			{
				int left = iX * cellSize;
				int right = cellSize + left;
				int top = iY * cellSize;
				int bottom = cellSize + top;

				cells[{iX, iY}] = new Cell{left, right, top, bottom};
			}
	}

	unordered_set<Cell*> World::calculateCells(Body* mBody)
	{
		int startX = mBody->getLeft() / cellSize + offset;
		int startY = mBody->getTop() / cellSize + offset;
		int endX = mBody->getRight() / cellSize + offset;
		int endY = mBody->getBottom() / cellSize + offset;

		unordered_set<Cell*> result;

		if (startX < 0 || endX >= columns || startY < 0 || endY >= rows)
		{
			//mBody.OnOutOfBounds.SafeInvoke();
			return result;
		}

		for(int iY{startY}; iY <= endY; iY++)
			for(int iX{startX}; iX <= endX; iX++)
				result.insert(cells[{iX, iY}]);

		return result;
	}

	void World::addBody(Body* mBody)
	{
		mBody->setCells(calculateCells(mBody));
		for(Cell* cell : mBody->getCells()) cell->addBody(mBody);
	}
	void World::delBody(Body* mBody) { for(Cell* cell : mBody->getCells()) cell->delBody(mBody); }
	void World::updateBody(Body* mBody) { delBody(mBody); addBody(mBody); }

	unordered_set<Body*> World::getBodies(Body* mBody)
	{
		unordered_set<Body*> result;

		for(Cell* cell : mBody->getCells())
			for(string group : mBody->getGroupsToCheck())
				for(Body* body : cell->getGroupedBodies(group))
					result.insert(body);

		return result;
	}
}

