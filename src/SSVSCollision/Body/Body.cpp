// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Resolver/ResolverBase.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/World/World.h"

using namespace std;
using namespace sf;
using namespace ssvu;
using namespace ssvsc::Utils;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vector2i mPosition, Vector2i mSize) : Base(mWorld), resolver(mWorld.getResolver()),
		shape{mPosition, mSize / 2}, oldShape{shape}, _static{mIsStatic} { }
	Body::~Body() { spatialInfo.destroy(); }

	void Body::addGroups(const vector<string>& mGroups)
	{
		for(const auto& g : mGroups) groupData.addUid(world.getGroupUid(g));
		groupData.addGroups(mGroups);
		spatialInfo.invalidate();
	}
	void Body::addGroupsToCheck(const vector<string>& mGroups)
	{
		for(const auto& g : mGroups) groupData.addUidToCheck(world.getGroupUid(g));
		groupData.addGroupsToCheck(mGroups);
		spatialInfo.invalidate();
	}
	void Body::addGroupsNoResolve(const vector<string>& mGroups)
	{
		for(const auto& g : mGroups) groupData.addUidNoResolve(world.getGroupUid(g));
		groupData.addGroupsNoResolve(mGroups);
	}

	void Body::update(float mFrameTime)
	{
		onPreUpdate();

		if(_static) { spatialInfo.preUpdate(); return; }
		if(outOfBounds) { onOutOfBounds(); outOfBounds = false; return; }

		oldShape = shape;
		integrate(mFrameTime);
		spatialInfo.preUpdate();
		bodiesToResolve.clear();

		for(const auto& body : spatialInfo.getBodiesToCheck())
		{
			if(body == this || !isOverlapping(shape, body->getShape())) continue;

			auto intersection(getMinIntersection(shape, body->getShape()));

			onDetection({*body, mFrameTime, body->getUserData(), intersection});
			body->onDetection({*this, mFrameTime, userData, -intersection});

			if(!resolve || containsAny(body->getGroupUids(), getGroupUidsNoResolve())) continue;
			bodiesToResolve.push_back(body);
		}

		if(!bodiesToResolve.empty()) resolver.resolve(*this, bodiesToResolve);
		if(oldShape != shape) spatialInfo.invalidate();

		spatialInfo.postUpdate();
		onPostUpdate();
	}

	void Body::integrate(float mFrameTime)
	{
		velocity += acceleration * mFrameTime;
		shape.move(Vector2i(velocity * mFrameTime));
		acceleration = {0, 0};
	}

	void Body::applyForce(sf::Vector2f mForce) { if(!_static) acceleration += mForce; }
	void Body::applyImpulse(sf::Vector2f mImpulse) { velocity += getInvMass() * mImpulse; }

	void Body::destroy() { world.delBody(this); Base::destroy(); }
}

