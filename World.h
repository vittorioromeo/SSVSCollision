#ifndef WORLD_H
#define WORLD_H

#include <vector>
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
			std::vector<Body*> bodies; // owned
			std::map<std::pair<int, int>, Cell*> cells; // TODO: multidimensional array

		public:
			World(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~World();

			void add(Body* mBody);
			void del(Body* mBody);
			void update(float mFrameTime);

			std::unordered_set<Cell*> calculateCells(Body* mBody);
			std::unordered_set<Body*> getBodies(Body* mBody);
	};
}

#endif // WORLD_H
