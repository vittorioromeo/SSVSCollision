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
				int left{iX * cellSize}, right{cellSize + left}, top{iY * cellSize}, bottom{cellSize + top};
				cells[{iX, iY}] = new Cell{left, right, top, bottom};
			}
	}
	World::~World()
	{
		for(Body* body : bodies) delete body;
		for(auto& pair : cells) delete pair.second;
	}

	void World::add(Body* mBody) { bodies.push_back(mBody); }
	void World::del(Body* mBody) { eraseFromVector(bodies, mBody); delete mBody; }
	void World::update(float mFrameTime) { for(Body* body : bodies) body->update(mFrameTime); }
}

