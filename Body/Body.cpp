#include "Body.h"
#include "World/World.h"
#include "Grid/Cell.h"
#include <sparsehash/dense_hash_set>

using namespace std;
using namespace sf;
using namespace google;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, int mWidth, int mHeight) : world(mWorld), grid(world.getGrid()),
		gridInfo{grid, *this}, isStatic{mIsStatic}, position{mPosition}, oldPosition{position}, halfSize{mWidth / 2, mHeight / 2} { }

	void Body::addGroups(const vector<string>& mGroups) { for(auto& group : mGroups) groups.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsToCheck(const vector<string>& mGroups) { for(auto& group : mGroups) groupsToCheck.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsNoResolve(const vector<string>& mGroups) { for(auto& group : mGroups) groupsNoResolve.push_back(group); }

	void Body::update(float mFrameTime)
	{
		gridInfo.preUpdate();
		if(isStatic) return;

		Vector2f tempVelocity{velocity.x * mFrameTime, velocity.y * mFrameTime};
		Vector2f tempPosition{position.x + tempVelocity.x, position.y + tempVelocity.y};
		oldPosition = position;
		position = Vector2i(tempPosition.x, tempPosition.y);

		// auto comp = [](Body* mBodyA, Body* mBodyB) -> bool { return mBodyA->getVelocity().x > 0 ? mBodyA->getX() > mBodyB->getX() : -mBodyA->getX() > mBodyB->getX(); };
		// set<Body*, decltype(comp)> bodiesToCheck(comp);
		// for(Body* body : getBodiesToCheck()) bodiesToCheck.insert(body);
		//bodiesToCheck.OrderBy(x => Velocity.X > 0 ? x.X : -x.X)

		for(auto& body : gridInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(body)) continue;

			onCollision({body, mFrameTime, body->getUserData()});
			body->onCollision({this, mFrameTime, userData});

			bool mustResolve{true};
			for(auto& group : groupsNoResolve) if(find(begin(body->getGroups()), end(body->getGroups()), group) != end(body->getGroups())) { mustResolve = false; break; }

			if(mustResolve) resolve(body);
		}

		gridInfo.postUpdate();
	}

	bool Body::isOverlapping(Body* mBody) { return getRight() > mBody->getLeft() && getLeft() < mBody->getRight() && (getBottom() > mBody->getTop() && getTop() < mBody->getBottom()); }
	void Body::resolve(Body* mBody)
	{
		int encrX{0}, encrY{0};

		if (getBottom() < mBody->getBottom() && getBottom() >= mBody->getTop()) encrY = mBody->getTop() - getBottom();
		else if (getTop() > mBody->getTop() && getTop() <= mBody->getBottom()) encrY = mBody->getBottom() - getTop();

		if (getLeft() < mBody->getLeft() && getRight() >= mBody->getLeft()) encrX = mBody->getLeft() - getRight();
		else if (getRight() > mBody->getRight() && getLeft() <= mBody->getRight()) encrX = mBody->getRight() - getLeft();

		int overlapX{getLeft() < mBody->getLeft() ? getRight() - mBody->getLeft() : mBody->getRight() - getLeft()};
		int overlapY{getTop() < mBody->getTop() ? getBottom() - mBody->getTop() : mBody->getBottom() - getTop()};

		if(overlapX > overlapY) position.y += encrY; else position.x += encrX;
	}
}

