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
		public:
			using BaseType = Base<TW>;
			using BodyType = Body<TW>;
			using ResolverType = typename TW::ResolverType;
			using SpatialInfoType = typename TW::SpatialInfoType;

		private:
			AABB shape;

		public:
			inline Sensor(TW& mWorld, const Vec2i& mPos, const Vec2i& mSize) noexcept : BaseType(mWorld), shape{mPos, mSize / 2} { this->spatialInfo.preUpdate(); }
			inline ~Sensor() noexcept { destroy(); }

			inline void update(float mFT) override
			{
				this->onPreUpdate();
				if(this->outOfBounds) { this->outOfBounds = false; return; };
				this->spatialInfo.preUpdate();
				this->spatialInfo.handleCollisions(mFT);
				this->spatialInfo.postUpdate();
			}
			inline void handleCollision(float mFT, BodyType* mBody) override
			{
				if(!this->mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;
				this->onDetection({*mBody, mBody->getUserData(), Utils::getMinIntersection(shape, mBody->getShape()), mFT});
			}
			inline void destroy() override { this->spatialInfo.destroy(); BaseType::destroy(); }

			inline void setPosition(const Vec2i& mPos)
			{
				if(mPos != shape.getPosition()) this->spatialInfo.invalidate();
				shape.setPosition(mPos);
			}

			inline AABB& getShape() noexcept override		{ return shape; }
			inline AABB& getOldShape() noexcept override	{ return shape; }
			inline BBType getType() const noexcept override	{ return BBType::Sensor; }
	};
}

#endif

