#include <algorithm>
#include <sparsehash/dense_hash_set>
#include <stack>
#include "Body.h"
#include "Grid/Cell.h"
#include "Utils/Utils.h"
#include "World/World.h"

using namespace std;
using namespace sf;
using namespace google;
using namespace ssvsc::Utils;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, int mWidth, int mHeight) : world(mWorld), grid(world.getGrid()),
		gridInfo{grid, *this}, shape{mPosition, {mWidth / 2, mHeight / 2}}, oldShape{shape}, isStatic{mIsStatic} { }

	void Body::addGroups(const vector<string>& mGroups) { for(auto& group : mGroups) groups.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsToCheck(const vector<string>& mGroups) { for(auto& group : mGroups) groupsToCheck.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsNoResolve(const vector<string>& mGroups) { for(auto& group : mGroups) groupsNoResolve.push_back(group); }

	void Body::update(float mFrameTime)
	{
		gridInfo.preUpdate();

		if(isStatic) return;
		integrate(mFrameTime);
		vector<AABB> shapesToResolve;

		for(auto& body : gridInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(shape, body->getShape())) continue;

			onCollision({body, mFrameTime, body->getUserData()});
			body->onCollision({this, mFrameTime, userData});

			if(!containsAny(body->getGroups(), groupsNoResolve)) shapesToResolve.push_back(body->getShape());
		}

		if(false)
		{
			sort(shapesToResolve, [&](const AABB& mA, const AABB& mB){ return getOverlapArea(shape, mA) > getOverlapArea(shape, mB); });
			for(auto& s : shapesToResolve) shape.setPosition(shape.getPosition() + getMinIntersection(shape, s));
		}
		else
		{
			vector<AABB> finalToResolve = shapesToResolve;

			if(oldShape.getX() > shape.getX()) finalToResolve = getMergedAABBSLeft(finalToResolve);
			else if(oldShape.getX() < shape.getX()) finalToResolve = getMergedAABBSRight(finalToResolve);

			if(oldShape.getY() > shape.getY()) finalToResolve = getMergedAABBSTop(finalToResolve);
			else if(oldShape.getY() < shape.getY()) finalToResolve = getMergedAABBSBottom(finalToResolve);

			vector<Vector2i> rVecs;
			for(auto& s : finalToResolve) rVecs.push_back(getMinIntersection(shape, s));
			if(!rVecs.empty()) shape.setPosition(shape.getPosition() + accumulate(begin(rVecs), end(rVecs), Vector2i{0, 0}) / (int) rVecs.size());
		}

		gridInfo.postUpdate();
	}

	void Body::integrate(float mFrameTime)
	{
		Vector2f tempVelocity{velocity.x * mFrameTime, velocity.y * mFrameTime};
		Vector2f tempPosition{shape.getX() + tempVelocity.x, shape.getY() + tempVelocity.y};
		oldShape = shape;
		shape.setPosition(Vector2i(tempPosition.x, tempPosition.y));
	}
}

