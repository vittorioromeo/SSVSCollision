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

		sort(shapesToResolve, [&](const AABB& mA, const AABB& mB){ return getOverlapArea(shape, mA) > getOverlapArea(shape, mB); });
		for(auto& shape : shapesToResolve) resolve(shape);

		gridInfo.postUpdate();
	}

	void Body::integrate(float mFrameTime)
	{
		Vector2f tempVelocity{velocity.x * mFrameTime, velocity.y * mFrameTime};
		Vector2f tempPosition{shape.getX() + tempVelocity.x, shape.getY() + tempVelocity.y};
		oldShape = shape;
		shape.setPosition(Vector2i(tempPosition.x, tempPosition.y));
	}

	void Body::resolve(const AABB& mShape)
	{
		int iX{getIntersectionX(shape, mShape)}, iY{getIntersectionY(shape, mShape)};
		if(abs(iX) > abs(iY)) shape.setPosition(shape.getPosition() + Vector2i{0, iY}); else shape.setPosition(shape.getPosition() + Vector2i{iX, 0});
	}
}

