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
	template<typename> class World;

	template<typename TS> class SensorType : public BaseType<TS>
	{
		public:
			using Base = BaseType<TS>;
			using Body = typename Base::Body;
			using Sensor = typename Base::Sensor;
			using Resolver = typename Base::Resolver;
			using DI = typename Base::DI;
			using RI = typename Base::RI;
			using SI = typename Base::SI;

		private:
			AABB shape;

		public:
			inline SensorType(World<TS>& mWorld, const Vec2i& mPosition, const Vec2i& mSize) : Base(mWorld), shape{mPosition, mSize / 2} { this->spatialInfo.preUpdate(); }
			inline ~SensorType() { destroy(); }

			inline void update(float mFT) override
			{
				this->onPreUpdate();
				if(this->outOfBounds) { this->outOfBounds = false; return; };
				this->spatialInfo.preUpdate();
				this->spatialInfo.handleCollisions(mFT);
				this->spatialInfo.postUpdate();
			}
			inline void handleCollision(float mFT, Body* mBody) override
			{
				if(!mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
				onDetection({*mBody, mBody->getUserData(), Utils::getMinIntersection(shape, mBody->getShape()), mFT});
			}
			inline void destroy() override { this->spatialInfo.destroy(); Base::destroy(); }

			inline void setPosition(const Vec2i& mPosition)
			{
				if(mPosition != shape.getPosition()) this->spatialInfo.invalidate();
				shape.setPosition(mPosition);
			}

			inline AABB& getShape() noexcept override		{ return shape; }
			inline AABB& getOldShape() noexcept override	{ return shape; }
			inline BType getType() const noexcept override	{ return BType::Sensor; }
	};
}

#endif

