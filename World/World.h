#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <sparsehash/dense_hash_set>
#include "Grid/Grid.h"

namespace ssvsc
{
	class Body;

	class World
	{
		private:
			std::vector<Body*> bodies; // owned
			google::dense_hash_set<Body*> bodiesToDel;
			Grid grid;

		public:
			World(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~World();

			void add(Body* mBody);
			void del(Body* mBody);
			void update(float mFrameTime);			

			Grid& getGrid();
	};
}

#endif // WORLD_H
