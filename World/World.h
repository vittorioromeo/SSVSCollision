#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <sparsehash/dense_hash_set>
#include <SFML/Graphics.hpp>
#include "Grid/Grid.h"

namespace ssvsc
{
	class Body;

	class World
	{
		friend class Body;

		private:
			std::vector<Body*> bodies; // owned
			google::dense_hash_set<Body*> bodiesToDel;
			Grid grid;

			void add(Body* mBody);
			void del(Body* mBody);
		
		public:
			World(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~World();

			Body* create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic);
			void update(float mFrameTime);			

			Grid& getGrid();
	};
}

#endif // WORLD_H
