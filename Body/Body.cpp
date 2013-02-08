// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include <stack>
#include "Body.h"
#include "Grid/Cell.h"
#include "Utils/Utils.h"
#include "World/World.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;

namespace ssvsc
{
	constexpr int resolutions = 1;

	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, Vector2i mSize) : world(mWorld), grid(world.getGrid()),
		gridInfo{grid, *this}, shape{mPosition, mSize / 2}, oldShape{shape}, isStatic{mIsStatic} { }

	void Body::addGroups(const vector<string>& mGroups) { for(auto& group : mGroups) groups.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsToCheck(const vector<string>& mGroups) { for(auto& group : mGroups) groupsToCheck.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsNoResolve(const vector<string>& mGroups) { for(auto& group : mGroups) groupsNoResolve.push_back(group); }

	void Body::update(float mFrameTime)
	{
		gridInfo.preUpdate();

		if(isStatic) return;

		oldShape = shape;
		integrate(mFrameTime);
		vector<AABB> shapesToResolve;

		for(auto& body : gridInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(shape, body->getShape())) continue;

			auto intersection = getIntersection(shape, body->getShape());

			onCollision({body, mFrameTime, body->getUserData(), intersection});
			body->onCollision({this, mFrameTime, userData, -intersection});

			if(!containsAny(body->getGroups(), groupsNoResolve)) shapesToResolve.push_back(body->getShape());
		}

		if(!shapesToResolve.empty())
		{
			if(velocity.x < 0) shapesToResolve = getMergedAABBSLeft(shapesToResolve);
			else if(velocity.x > 0) shapesToResolve = getMergedAABBSRight(shapesToResolve);

			if(velocity.y < 0) shapesToResolve = getMergedAABBSTop(shapesToResolve);
			else if(velocity.y > 0) shapesToResolve = getMergedAABBSBottom(shapesToResolve);

			for(int i{0}; i < resolutions; ++i)
			{
				Vector2i rVec{0, 0};
				for(auto& s : shapesToResolve) rVec += getMinIntersection(shape, s);

				shape.move(rVec);
				if(abs(rVec.x) > abs(rVec.y)) velocity.x = 0; else velocity.y = 0;
				
				//velocity = Vector2f(rVec);
				//shape.move(rVec);
				//if(abs(rVec.x) > abs(rVec.y)) velocity.x = 0; else velocity.y = 0;
				//shape.move(rVec / static_cast<int>(shapesToResolve.size()));
				//applyForce(Vector2f(rVec) / 4.f);
				//velocity = Vector2f(rVec / static_cast<int>(shapesToResolve.size()));
				//velocity = Vector2f(rVec);
				//velocity = Vector2f(rVec);
				//acceleration = Vector2f(rVec) * 0.5f;
			}
		}

		gridInfo.postUpdate();
	}

	void Body::integrate(float mFrameTime)
	{
		velocity += acceleration * mFrameTime;
		shape.move(Vector2i(velocity * mFrameTime));

		acceleration = {0, 0};
	}

	void Body::applyForce(sf::Vector2f mForce) { acceleration += mForce; }

	void Body::destroy() { world.del(this); }
}

