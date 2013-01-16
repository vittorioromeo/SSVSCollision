#include "Body.h"
#include "World.h"
#include "Cell.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, int mWidth, int mHeight) :
		world(mWorld), isStatic{mIsStatic}, position{mPosition}, previousPosition{position}, halfSize{mWidth / 2, mHeight / 2} { }

	void Body::addGroups(const vector<string>& mGroups)
	{
		for(auto group : mGroups) groups.insert(group);
		checkCells(); recalculateCells();
	}
	void Body::addGroupsToCheck(const vector<string>& mGroups) { for(auto group : mGroups) groupsToCheck.insert(group); }
	void Body::addGroupsNoResolve(const vector<string>& mGroups) { for(auto group : mGroups) groupsNoResolve.insert(group); }

	bool Body::isOverlapping(Body* mBody) { return getRight() > mBody->getLeft() && getLeft() < mBody->getRight() && (getBottom() > mBody->getTop() && getTop() < mBody->getBottom()); }
	void Body::update(float mFrameTime)
	{
		if(isStatic) return;

		

		Vector2f tempVelocity{velocity.x * mFrameTime, velocity.y * mFrameTime};
		Vector2f tempPosition{position.x + tempVelocity.x, position.y + tempVelocity.y};
		previousPosition = position;
		position = Vector2i(tempPosition.x, tempPosition.y);

		unordered_set<Body*> checkedBodies{this};

		// auto comp = [](Body* mBodyA, Body* mBodyB) -> bool { return mBodyA->getVelocity().x > 0 ? mBodyA->getX() > mBodyB->getX() : -mBodyA->getX() > mBodyB->getX(); };
		// set<Body*, decltype(comp)> bodiesToCheck(comp);
		// for(Body* body : getBodiesToCheck()) bodiesToCheck.insert(body);
		//bodiesToCheck.OrderBy(x => Velocity.X > 0 ? x.X : -x.X)

		for(Body* body : getBodiesToCheck())
		{
			if(checkedBodies.find(body) != checkedBodies.end()) continue;
			checkedBodies.insert(body);

			if(!isOverlapping(body)) continue;

			onCollision({body, mFrameTime, body->getUserData()});
			body->onCollision({this, mFrameTime, userData});

			for(auto g : groupsNoResolve) if(body->getGroups().find(g) != body->getGroups().end()) continue;

			int encrX{0}, encrY{0};

			if (getBottom() < body->getBottom() && getBottom() >= body->getTop()) encrY = body->getTop() - getBottom();
			else if (getTop() > body->getTop() && getTop() <= body->getBottom()) encrY = body->getBottom() - getTop();

			if (getLeft() < body->getLeft() && getRight() >= body->getLeft()) encrX = body->getLeft() - getRight();
			else if (getRight() > body->getRight() && getLeft() <= body->getRight()) encrX = body->getRight() - getLeft();

			int overlapX{getLeft() < body->getLeft() ? getRight() - body->getLeft() : body->getRight() - getLeft()};
			int overlapY{getTop() < body->getTop() ? getBottom() - body->getTop() : body->getBottom() - getTop()};

			if(overlapX > overlapY) position.y += encrY; else position.x += encrX;
		}

		checkCells();
	}

	void Body::checkCells()
	{
		startX = getLeft() / world.cellSize;
		startY = getTop() / world.cellSize;
		endX = getRight() / world.cellSize;
		endY = getBottom() / world.cellSize;

		previousStartX = (previousPosition.x - halfSize.x) / world.cellSize;
		if(previousStartX != startX) { recalculateCells(); return; }

		previousStartY = (previousPosition.y - halfSize.y) / world.cellSize;
		if(previousStartY != startY) { recalculateCells(); return; }

		previousEndX = (previousPosition.x + halfSize.x) / world.cellSize;
		if(previousEndX != endX) { recalculateCells(); return; }

		previousEndY = (previousPosition.y + halfSize.y)	/ world.cellSize;
		if(previousEndY != endY) { recalculateCells(); return; }
	}

	void Body::recalculateCells()
	{
		for(Cell* cell : cells) cell->del(this);

		cells.clear();
		if(startX < 0 || endX >= world.columns || startY < 0 || endY >= world.rows) { onOutOfBounds(); }
		for(int iY{startY}; iY <= endY; iY++) for(int iX{startX}; iX <= endX; iX++) cells.insert(world.cells[{iX + world.offset, iY + world.offset}]);

		for(Cell* cell : cells) cell->add(this);
	}

	unordered_set<Body*> Body::getBodiesToCheck()
	{
		unordered_set<Body*> result;
		for(Cell* cell : cells) for(auto group : groupsToCheck) for(Body* body : cell->getBodies(group)) result.insert(body);
		return result;
	}

	// Properties
	void Body::setPosition(Vector2i mPosition) 			{ position = mPosition; }
	void Body::setVelocity(Vector2i mVelocity) 			{ velocity = mVelocity; }
	unordered_set<string> Body::getGroups()				{ return groups; }
	Vector2i Body::getPosition() 						{ return position; }
	Vector2i Body::getVelocity() 						{ return velocity; }
	void* Body::getUserData()							{ return userData; }
	int Body::getX() 									{ return position.x; }
	int Body::getY() 									{ return position.y; }
	int Body::getLeft() 								{ return position.x - halfSize.x; }
	int Body::getRight() 								{ return position.x + halfSize.x; }
	int Body::getTop() 									{ return position.y - halfSize.y; }
	int Body::getBottom() 								{ return position.y + halfSize.y; }
	int Body::getHalfWidth() 							{ return halfSize.x; }
	int Body::getHalfHeight() 							{ return halfSize.y; }
	int Body::getWidth() 								{ return halfSize.x * 2; }
	int Body::getHeight() 								{ return halfSize.y * 2; }
}

