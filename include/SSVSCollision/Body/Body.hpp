// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY
#define SSVSC_BODY

#include "SSVSCollision/AABB/AABB.hpp"
#include "SSVSCollision/Body/CallbackInfo.hpp"
#include "SSVSCollision/Body/BodyData.hpp"
#include "SSVSCollision/Body/Base.hpp"
#include "SSVSCollision/Utils/UtilsAABB.hpp"
#include "SSVSCollision/Utils/Utils.hpp"

namespace ssvsc
{
	template<typename TW> class Body : public Base<TW>, public TW::ResolverInfoType, public RestitutionData
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

		protected:
			AABB shape, oldShape;
			bool _static, resolve{true};
			Vec2f velocity, oldVelocity, acceleration;
			MassData massData;
			void* userData{nullptr};
			std::vector<Body*> toResolve;
			Vec2i lastResolution;
			bool mustInit{true};

			inline void integrate(FT mFT)
			{
				velocity += acceleration * mFT;
				shape.move(Vec2i(velocity * mFT));
				ssvs::nullify(acceleration);
			}

			inline void update(FT mFT)
			{
				if(mustInit) { this->spatialInfo.template init<BodyTag>(); mustInit = false; }

				ssvs::nullify(lastResolution);

				this->onPreUpdate();

				if(_static) { this->spatialInfo.template preUpdate<BodyTag>(); return; }
				if(this->outOfBounds) { onOutOfBounds(); this->outOfBounds = false; return; }
				oldShape = shape;
				oldVelocity = velocity;
				integrate(mFT);
				this->spatialInfo.template preUpdate<BodyTag>();

				toResolve.clear();
				this->spatialInfo.template handleCollisions<BodyTag>(mFT);

				this->world.resolver.resolve(*this, toResolve);
				if(oldShape != shape) this->spatialInfo.invalidate();

				this->spatialInfo.postUpdate(); onPostUpdate();
			}

			inline void handleCollision(FT mFT, Body* mBody)
			{
				if(mBody == this || !this->mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;

				this->onDetection({*mBody, mBody->getUserData(), mFT});
				mBody->onDetection({*this, userData, mFT});

				if(mustResolveAgainst(*mBody)) toResolve.emplace_back(mBody);
			}

		public:
			using RestitutionData::RestitutionData;
			using ResolverInfoType::ResolverInfoType;

			ssvu::Delegate<void()> onPostUpdate, onOutOfBounds;
			ssvu::Delegate<void(const ResolutionInfoType&)> onResolution;

			inline Body(TW& mWorld, bool mIsStatic, const Vec2i& mPos, const Vec2i& mSize) noexcept : Base<TW>{mWorld}, ResolverInfoType{*this}, shape{mPos, mSize / 2}, oldShape{shape}, _static{mIsStatic} { }
			inline ~Body() noexcept { destroy(); }
			inline void destroy() { this->spatialInfo.template destroy<BodyTag>(); this->world.delBody(*this); }

			inline void applyAccel(const Vec2f& mAccel) noexcept		{ acceleration += mAccel; }
			inline void resolvePosition(const Vec2i& mOffset) noexcept	{ shape.move(mOffset); lastResolution += mOffset; }

			inline void setPosition(const Vec2i& mPos)					{ oldShape = shape; shape.setPosition(mPos);	this->spatialInfo.invalidate(); }
			inline void setX(int mX)									{ oldShape = shape; shape.setX(mX);				this->spatialInfo.invalidate(); }
			inline void setY(int mY)									{ oldShape = shape; shape.setY(mY);				this->spatialInfo.invalidate(); }
			inline void setSize(const Vec2i& mSize)						{ shape.setSize(mSize);							this->spatialInfo.invalidate(); }
			inline void setHalfSize(const Vec2i& mSize)					{ shape.setHalfSize(mSize);						this->spatialInfo.invalidate(); }
			inline void setWidth(int mWidth)							{ shape.setWidth(mWidth);						this->spatialInfo.invalidate(); }
			inline void setHeight(int mHeight)							{ shape.setHeight(mHeight);						this->spatialInfo.invalidate(); }
			inline void setStatic(bool mStatic)							{ _static = mStatic;							this->spatialInfo.invalidate(); }
			inline void setVelocity(const Vec2f& mVel) noexcept			{ velocity = mVel; }
			inline void setAcceleration(const Vec2f& mAccel) noexcept	{ acceleration = mAccel; }
			inline void setUserData(void* mUserData) noexcept			{ userData = mUserData; }
			inline void setVelocityX(float mX) noexcept					{ velocity.x = mX; }
			inline void setVelocityY(float mY) noexcept					{ velocity.y = mY; }
			inline void setResolve(bool mResolve) noexcept				{ resolve = mResolve; }
			inline void setMass(float mMass) noexcept					{ massData.setMass(mMass); }

			inline AABB& getShape() noexcept							{ return shape; }
			inline AABB& getOldShape() noexcept							{ return oldShape; }
			inline const Vec2i& getPosition() const noexcept			{ return shape.getPosition(); }
			inline const Vec2f& getVelocity() const noexcept			{ return velocity; }
			inline const Vec2i& getOldPosition() const noexcept			{ return oldShape.getPosition(); }
			inline const Vec2f& getOldVelocity() const noexcept			{ return oldVelocity; }
			inline const Vec2f& getAcceleration() const noexcept		{ return acceleration; }
			inline Vec2i getSize() const noexcept						{ return shape.getSize(); }
			inline float getMass() const noexcept						{ return _static ? 0 : massData.getMass(); }
			inline float getInvMass() const noexcept					{ return _static ? 0 : massData.getInvMass(); }
			inline int getWidth() const noexcept						{ return shape.getWidth(); }
			inline int getHeight() const noexcept						{ return shape.getHeight(); }
			inline bool isStatic() const noexcept						{ return _static; }
			inline void* getUserData() const noexcept					{ return userData; }
			inline bool hasMovedLeft() const noexcept					{ return shape.getX() < oldShape.getX(); }
			inline bool hasMovedRight() const noexcept					{ return shape.getX() > oldShape.getX(); }
			inline bool hasMovedUp() const noexcept						{ return shape.getY() < oldShape.getY(); }
			inline bool hasMovedDown() const noexcept					{ return shape.getY() > oldShape.getY(); }
			inline bool getResolve() const noexcept						{ return resolve; }
			inline const Vec2i& getLastResolution() const noexcept		{ return lastResolution; }

			inline bool mustResolveAgainst(const Body& mBody) const noexcept { return resolve && !this->mustIgnoreResolution(mBody); }
	};
}

#endif

// TODO: bodystate data-only class
