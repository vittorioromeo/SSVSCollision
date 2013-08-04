// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SENSOR
#define SSVSC_SENSOR

#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Utils/Utils.h"

namespace ssvsc
{
	class World;

	class Sensor : public Base
	{
		private:
			AABB shape;

		public:
			ssvu::Delegate<void()> onPreUpdate;
			ssvu::Delegate<void(const DetectionInfo&)> onDetection;

			Sensor(World& mWorld, Vec2i mPosition, Vec2i mSize) : Base(mWorld), shape{mPosition, mSize / 2} { spatialInfo.preUpdate(); }
			~Sensor() { spatialInfo.destroy(); }

			inline void update(float mFrameTime) override
			{
				onPreUpdate();
				if(outOfBounds) { outOfBounds = false; return; };
				spatialInfo.preUpdate();
				spatialInfo.handleCollisions(mFrameTime);
				spatialInfo.postUpdate();
			}
			inline void handleCollision(float mFrameTime, Body* mBody) override
			{
				if(!mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
				onDetection({*mBody, mFrameTime, mBody->getUserData(), ssvsc::Utils::getMinIntersection(shape, mBody->getShape())});
			}
			inline AABB& getShape() override	{ return shape; }
			inline AABB& getOldShape() override	{ return shape; }
			inline Type getType() override		{ return Type::Sensor; }
			inline void destroy() override		{ Base::destroy(); }

			inline void setPosition(Vec2i mPosition)
			{
				if(mPosition != shape.getPosition()) spatialInfo.invalidate();
				shape.setPosition(mPosition);
			}
	};
}

#endif

