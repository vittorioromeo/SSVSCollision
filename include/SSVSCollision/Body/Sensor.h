// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SENSOR
#define SSVSC_SENSOR

#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Utils/Utils.h"

namespace ssvsc
{
	template<typename TW> class Sensor : public Base<TW>
	{
		private:
			AABB shape;

		public:
			inline Sensor(TW& mWorld, const Vec2i& mPos, const Vec2i& mSize) noexcept : Base<TW>{mWorld}, shape{mPos, mSize / 2} { this->spatialInfo.template preUpdate<SensorTag>(); }
			inline ~Sensor() noexcept { destroy(); }
			inline void destroy() { this->spatialInfo.template destroy<SensorTag>(); this->world.delSensor(*this); }

			inline void update(float mFT)
			{
				this->onPreUpdate();
				if(this->outOfBounds) { this->outOfBounds = false; return; };
				this->spatialInfo.template preUpdate<SensorTag>();
				this->spatialInfo.template handleCollisions<SensorTag>(mFT);
				this->spatialInfo.postUpdate();
			}
			inline void handleCollision(float mFT, Body<TW>* mBody)
			{
				if(!this->mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
				this->onDetection({*mBody, mBody->getUserData(), mFT});
			}

			inline void setPosition(const Vec2i& mPos)
			{
				if(mPos != shape.getPosition()) this->spatialInfo.invalidate();
				shape.setPosition(mPos);
			}

			inline AABB& getShape() noexcept { return shape; }
	};
}

#endif

