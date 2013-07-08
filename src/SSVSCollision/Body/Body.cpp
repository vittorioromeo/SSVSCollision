// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Resolver/ResolverBase.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/World/World.h"

using namespace std;
using namespace ssvu;
using namespace ssvsc::Utils;

namespace ssvsc
{
	Body::Body(World& mWorld, bool mIsStatic, Vec2i mPosition, Vec2i mSize) : Base(mWorld), resolver(mWorld.getResolver()), shape{mPosition, mSize / 2}, oldShape{shape}, _static{mIsStatic} { spatialInfo.preUpdate(); }
	Body::~Body() { spatialInfo.destroy(); }

	void Body::handleCollision(float mFrameTime, Body* mBody)
	{
		if(mBody == this || !mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;

		auto intersection(getMinIntersection(shape, mBody->getShape()));

		onDetection({*mBody, mFrameTime, mBody->getUserData(), intersection});
		mBody->onDetection({*this, mFrameTime, userData, -intersection});

		if(!resolve || mustIgnoreResolution(*mBody)) return;
		bodiesToResolve.push_back(mBody);
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
		spatialInfo.handleCollisions(mFrameTime);

		if(!bodiesToResolve.empty()) resolver.resolve(*this, bodiesToResolve);
		if(oldShape != shape) spatialInfo.invalidate();

		spatialInfo.postUpdate();
		onPostUpdate();
	}

	void Body::integrate(float mFrameTime)
	{
		velocity += acceleration * mFrameTime;
		shape.move(Vec2i(velocity * mFrameTime));
		acceleration = {0, 0};
	}

	void Body::destroy() { Base::destroy(); }
}

