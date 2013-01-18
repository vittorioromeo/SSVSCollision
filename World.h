#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <map>
#include <unordered_set>
#include <array>

namespace ssvsc
{
	class Body;
	class Cell;

	class World
	{
		private:
			std::vector<Body*> bodies; // owned
			std::unordered_set<Body*> bodiesToDel;
			std::vector<std::vector<Cell*>> cells;
			int columns, rows, cellSize, offset;

		public:
			World(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~World();

			void add(Body* mBody);
			void del(Body* mBody);
			void update(float mFrameTime);

			Cell* getCell(int mX, int mY);
			int getColumns();
			int getRows();
			int getCellSize();
			int getOffset();
	};
}

#endif // WORLD_H
