// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SENSOR
#define SSVSC_SENSOR

#include "SSVSCollision/AABB/AABB.hpp"
#include "SSVSCollision/Body/CallbackInfo.hpp"
#include "SSVSCollision/Body/Base.hpp"
#include "SSVSCollision/Body/Body.hpp"
#include "SSVSCollision/Utils/Utils.hpp"

namespace ssvsc
{
	template<typename TW> class Sensor : public Base<TW>
	{
		public:
			using SpatialInfoType = typename TW::SpatialInfoType;
			using DetectionInfoType = typename TW::DetectionInfoType;
			using ResolverType = typename TW::ResolverType;
			using ResolverInfoType = typename TW::ResolverInfoType;
			using ResolutionInfoType = typename TW::ResolutionInfoType;

			friend TW;
			friend SpatialInfoType;
			friend ResolverType;
			friend ResolverInfoType;

		private:
			AABB shape;

			inline void update(FT mFT)
			{
				this->onPreUpdate();
				if(this->outOfBounds) { this->outOfBounds = false; return; };
				this->spatialInfo.template preUpdate<SensorTag>();
				this->spatialInfo.template handleCollisions<SensorTag>(mFT);
				this->spatialInfo.postUpdate();
			}
			inline void handleCollision(FT mFT, Body<TW>* mBody)
			{
				if(!this->mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
				this->onDetection({*mBody, mBody->getUserData(), mFT});
			}

		public:
			inline Sensor(TW& mWorld, const Vec2i& mPos, const Vec2i& mSize) noexcept : Base<TW>{mWorld}, shape{mPos, mSize / 2} { this->spatialInfo.template preUpdate<SensorTag>(); }
			inline ~Sensor() noexcept { destroy(); }
			inline void destroy() { this->spatialInfo.template destroy<SensorTag>(); this->world.delSensor(*this); }

			inline void setPosition(const Vec2i& mPos)
			{
				if(mPos != shape.getPosition()) this->spatialInfo.invalidate();
				shape.setPosition(mPos);
			}

			inline AABB& getShape() noexcept { return shape; }
	};
}

#endif

