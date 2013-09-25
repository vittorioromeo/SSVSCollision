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
			inline Sensor(World& mWorld, const Vec2i& mPosition, const Vec2i& mSize) : Base(mWorld), shape{mPosition, mSize / 2} { spatialInfo.preUpdate(); }
			inline ~Sensor() { destroy(); }

			inline void update(float mFT) override
			{
				onPreUpdate();
				if(outOfBounds) { outOfBounds = false; return; };
				spatialInfo.preUpdate();
				spatialInfo.handleCollisions(mFT);
				spatialInfo.postUpdate();
			}
			inline void handleCollision(float mFT, Body* mBody) override
			{
				if(!mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
				onDetection({*mBody, mBody->getUserData(), Utils::getMinIntersection(shape, mBody->getShape()), mFT});
			}
			inline void destroy() override { spatialInfo.destroy(); Base::destroy(); }

			inline void setPosition(const Vec2i& mPosition)
			{
				if(mPosition != shape.getPosition()) spatialInfo.invalidate();
				shape.setPosition(mPosition);
			}

			inline AABB& getShape() noexcept override			{ return shape; }
			inline AABB& getOldShape() noexcept override		{ return shape; }
			inline BaseType getType() const noexcept override	{ return BaseType::Sensor; }
	};
}

#endif

