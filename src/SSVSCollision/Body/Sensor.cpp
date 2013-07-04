// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Body/Sensor.h"
#include "SSVSCollision/Utils/Utils.h"

using namespace std;
using namespace ssvu;
using namespace ssvsc::Utils;

namespace ssvsc
{
	void Sensor::handleCollision(float mFrameTime, Body* mBody)
	{
		if(!mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
		onDetection({*mBody, mFrameTime, mBody->getUserData(), getMinIntersection(shape, mBody->getShape())});
	}

	void Sensor::update(float mFrameTime)
	{
		onPreUpdate();
		if(outOfBounds) { outOfBounds = false; return; };
		spatialInfo.preUpdate();
		spatialInfo.handleCollisions(mFrameTime);
		spatialInfo.postUpdate();
	}
}
