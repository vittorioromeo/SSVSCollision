// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include <stack>
#include "Body.h"
#include "Grid/Cell.h"
#include "Utils/Merge.h"
#include "Utils/Utils.h"
#include "World/World.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, Vector2i mSize) : world(mWorld), grid(world.getGrid()),
		gridInfo{grid, *this}, shape{mPosition, mSize / 2}, oldShape{shape}, _static{mIsStatic} { }

	void Body::addGroups(const vector<string>& mGroups) { for(auto& group : mGroups) groups.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsToCheck(const vector<string>& mGroups) { for(auto& group : mGroups) groupsToCheck.push_back(group); gridInfo.invalidate(); }
	void Body::addGroupsNoResolve(const vector<string>& mGroups) { for(auto& group : mGroups) groupsNoResolve.push_back(group); }

	void Body::update(float mFrameTime)
	{
		gridInfo.preUpdate();

		if(_static) return;

		oldShape = shape;
		integrate(mFrameTime);
		vector<AABB> shapesToResolve;

		for(auto& body : gridInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(shape, body->getShape())) continue;

			auto intersection = getMinIntersection(shape, body->getShape());

			onCollision({*body, mFrameTime, body->getUserData(), intersection});
			body->onCollision({*this, mFrameTime, userData, -intersection});

			if(!containsAny(body->getGroups(), groupsNoResolve)) shapesToResolve.push_back(body->getShape());
		}

		if(!shapesToResolve.empty())
		{
			if(velocity.x < 0) shapesToResolve = getMergedAABBs<Side::Left>(shapesToResolve);
			else if(velocity.x > 0) shapesToResolve = getMergedAABBs<Side::Right>(shapesToResolve);

			if(velocity.y < 0) shapesToResolve = getMergedAABBs<Side::Top>(shapesToResolve);
			else if(velocity.y > 0) shapesToResolve = getMergedAABBs<Side::Bottom>(shapesToResolve);

			for(auto& s : shapesToResolve)
			{
				bool notResolved{true};

				if(oldShape.isLeftOf(s))
				{
					shape.move({s.getLeft() - shape.getRight(), 0});
					velocity.x = 0; notResolved = false;
				}
				else if(oldShape.isRightOf(s))
				{
					shape.move({s.getRight() - shape.getLeft(), 0});
					velocity.x = 0; notResolved = false;
				}

				if(oldShape.isAbove(s))
				{
					shape.move({0, s.getTop() - shape.getBottom()});
					velocity.y = 0; continue;
				}
				else if(oldShape.isBelow(s))
				{
					shape.move({0, s.getBottom() - shape.getTop()});
					velocity.y = 0; continue;
				}

				if(notResolved) shape.move(getMin1DIntersection(shape, s));
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

