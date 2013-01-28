#include "Body.h"
#include "Grid/Cell.h"
#include "Utils/Utils.h"
#include "World/World.h"
#include <sparsehash/dense_hash_set>

using namespace std;
using namespace sf;
using namespace google;
using namespace ssvsc::Utils;

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

		vector<Body*> bodiesToResolve;

		for(auto& body : gridInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(this, body)) continue;

			onCollision({body, mFrameTime, body->getUserData()});
			body->onCollision({this, mFrameTime, userData});

			if(!containsAny(body->getGroups(), groupsNoResolve)) bodiesToResolve.push_back(body);
		}

		sort(bodiesToResolve, [&](Body* mBodyA, Body* mBodyB){ return overlapAreaComparer(this, mBodyA, mBodyB); });

		for(auto& body : bodiesToResolve) resolve(body);
		gridInfo.postUpdate();
	}
	
	void Body::resolve(Body* mBody)
	{
		int encrX{0}, encrY{0};

		if (getBottom() < mBody->getBottom() && getBottom() >= mBody->getTop()) encrY = mBody->getTop() - getBottom();
		else if (getTop() > mBody->getTop() && getTop() <= mBody->getBottom()) encrY = mBody->getBottom() - getTop();

		if (getLeft() < mBody->getLeft() && getRight() >= mBody->getLeft()) encrX = mBody->getLeft() - getRight();
		else if (getRight() > mBody->getRight() && getLeft() <= mBody->getRight()) encrX = mBody->getRight() - getLeft();

		if(getOverlapX(this, mBody) > getOverlapY(this, mBody)) position.y += encrY; else position.x += encrX;
	}
}
