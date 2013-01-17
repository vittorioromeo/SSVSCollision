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
			std::vector<Body*> bodies; // owned
			std::unordered_set<Body*> bodiesToDel;
			
		public:
			int columns, rows, cellSize, offset; // TODO: private
			std::map<std::pair<int, int>, Cell*> cells; // TODO: multidimensional array
			
			World(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~World();

			void add(Body* mBody);
			void del(Body* mBody);
			void update(float mFrameTime);
	};
}

#endif // WORLD_H
