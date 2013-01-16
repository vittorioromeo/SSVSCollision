#ifndef WORLD_H
#define WORLD_H

#include <map>
#include <unordered_set>

namespace ssvsc
{
	class Body;
	class Cell;

	class World
	{
		private:
			int columns, rows, cellSize, offset;
			std::map<std::pair<int, int>, Cell*> cells;

		public:
			World(int mColumns, int mRows, int mCellSize, int mOffset = 0);			

			void addBody(Body* mBody);
			void delBody(Body* mBody);
			void updateBody(Body* mBody);
			std::unordered_set<Cell*> calculateCells(Body* mBody);
			std::unordered_set<Body*> getBodies(Body* mBody);
	};
}

#endif // WORLD_H
