#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <map>
#include <array>
#include <sparsehash/dense_hash_set>

namespace ssvsc
{
	class Body;
	class Cell;

	class World
	{
		private:
			std::vector<Body*> bodies; // owned
			std::vector<std::vector<Cell*>> cells; // owned
			google::dense_hash_set<Body*> bodiesToDel;			
			int columns, rows, cellSize, offset;

		public:
			World(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~World();

			void add(Body* mBody);
			void del(Body* mBody);
			void update(float mFrameTime);			

			Cell* getCell(int mX, int mY);
			int getIndex(int mValue);
			bool isOutside(int mStartX, int mStartY, int mEndX, int mEndY);
	};
}

#endif // WORLD_H
