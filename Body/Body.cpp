// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <algorithm>
#include <stack>
#include "Body.h"
#include "Resolver/ResolverBase.h"
#include "Spatial/SpatialInfoBase.h"
#include "Utils/Utils.h"
#include "World/World.h"

using namespace std;
using namespace sf;
using namespace ssvu::Utils;
using namespace ssvsc::Utils;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, Vector2i mSize) : world(mWorld), resolver(mWorld.getResolver()),
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
		vector<Body*> bodiesToResolve;

		for(auto& body : spatialInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(shape, body->getShape())) continue;

			auto intersection = getMinIntersection(shape, body->getShape());

			onDetection({*body, mFrameTime, body->getUserData(), intersection});
			body->onDetection({*this, mFrameTime, userData, -intersection});

			if(!containsAny(body->getGroups(), groupsNoResolve)) bodiesToResolve.push_back(body);
		}

		if(!bodiesToResolve.empty()) resolver.resolve(*this, bodiesToResolve);

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

