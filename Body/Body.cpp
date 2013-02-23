// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include <stack>
#include "Body.h"
#include "Utils/Traits.h"
#include "Utils/Utils.h"
#include "World/World.h"
#include "Spatial/SpatialInfoBase.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Traits;
using namespace ssvsc::Utils;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, Vector2i mSize) : world(mWorld),
		spatialInfo(world.getSpatial().createSpatialInfo(*this)), shape{mPosition, mSize / 2}, oldShape{shape}, _static{mIsStatic} { }
	Body::~Body() { spatialInfo.destroy(); }

	void Body::addGroups(const vector<string>& mGroups) { for(auto& group : mGroups) groups.push_back(group); spatialInfo.invalidate(); }
	void Body::addGroupsToCheck(const vector<string>& mGroups) { for(auto& group : mGroups) groupsToCheck.push_back(group); spatialInfo.invalidate(); }
	void Body::addGroupsNoResolve(const vector<string>& mGroups) { for(auto& group : mGroups) groupsNoResolve.push_back(group); }

	void Body::update(float mFrameTime)
	{
		onPreUpdate();
		spatialInfo.preUpdate();

		if(_static) return;
		if(outOfBounds) { onOutOfBounds(); outOfBounds = false; return; }

		oldShape = shape;
		integrate(mFrameTime);
		vector<AABB> shapesToResolve;

		for(auto& body : spatialInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(shape, body->getShape())) continue;

			auto intersection = getMinIntersection(shape, body->getShape());

			onDetection({*body, mFrameTime, body->getUserData(), intersection});
			body->onDetection({*this, mFrameTime, userData, -intersection});

			if(!containsAny(body->getGroups(), groupsNoResolve)) shapesToResolve.push_back(body->getShape());
		}

		if(!shapesToResolve.empty())
		{
			if(velocity.x < 0) shapesToResolve = getMergedAABBs<Merge::Left>(shapesToResolve);
			else if(velocity.x > 0) shapesToResolve = getMergedAABBs<Merge::Right>(shapesToResolve);

			if(velocity.y < 0) shapesToResolve = getMergedAABBs<Merge::Top>(shapesToResolve);
			else if(velocity.y > 0) shapesToResolve = getMergedAABBs<Merge::Bottom>(shapesToResolve);

			for(auto& s : shapesToResolve)
			{
				bool notResolved{true};

				if(oldShape.isLeftOf(s)) { resolve<Resolution::LeftOf>(s); notResolved = false; }
				else if(oldShape.isRightOf(s)) { resolve<Resolution::RightOf>(s); notResolved = false; }

				if(oldShape.isAbove(s)) { resolve<Resolution::Above>(s); continue; }
				else if(oldShape.isBelow(s)) { resolve<Resolution::Below>(s); continue; }

				if(notResolved) resolve<Resolution::General>(s);
			}
		}

		spatialInfo.postUpdate();
		onPostUpdate();
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

