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
	template<typename TW> class Body : public Base<TW>, public TW::ResolverInfoType
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
			BodyData bodyData;
			std::vector<Body*> toResolve;
			void* userData{nullptr};
			bool mustInit{true};

			inline void integrate(FT mFT) noexcept
			{
				bodyData.velocity += getAcceleration() * mFT;
				getShape().move(Vec2i(getVelocity() * mFT));
				ssvs::nullify(bodyData.acceleration);
			}

			inline void update(FT mFT)
			{
				if(mustInit) { this->spatialInfo.template init<BodyTag>(); mustInit = false; }

				ssvs::nullify(bodyData.lastResolution);

				this->onPreUpdate();

				if(isStatic()) { this->spatialInfo.template preUpdate<BodyTag>(); return; }
				if(this->outOfBounds) { onOutOfBounds(); this->outOfBounds = false; return; }
				bodyData.oldShape = getShape();
				bodyData.oldVelocity = getVelocity();
				integrate(mFT);
				this->spatialInfo.template preUpdate<BodyTag>();

				toResolve.clear();
				this->spatialInfo.template handleCollisions<BodyTag>(mFT);

				this->world.resolver.resolve(*this, toResolve);
				if(getOldShape() != getShape()) this->spatialInfo.invalidate();

				this->spatialInfo.postUpdate(); onPostUpdate();
			}

			inline void handleCollision(FT mFT, Body* mBody)
			{
				if(mBody == this || !this->mustCheck(*mBody) || !getShape().isOverlapping(mBody->getShape())) return;

				this->onDetection({*mBody, mBody->getUserData(), mFT});
				mBody->onDetection({*this, userData, mFT});

				if(mustResolveAgainst(*mBody)) toResolve.emplace_back(mBody);
			}

		public:
			using ResolverInfoType::ResolverInfoType;

			ssvu::Delegate<void()> onPostUpdate, onOutOfBounds;
			ssvu::Delegate<void(const ResolutionInfoType&)> onResolution;

			inline Body(TW& mWorld, bool mIsStatic, const Vec2i& mPos, const Vec2i& mSize) noexcept : Base<TW>{mWorld}, ResolverInfoType{*this}, bodyData{mIsStatic, mPos, mSize} { }
			inline ~Body() noexcept { destroy(); }
			inline void destroy() { this->spatialInfo.template destroy<BodyTag>(); this->world.delBody(*this); }

			inline void applyAccel(const Vec2f& mAccel) noexcept		{ bodyData.acceleration += mAccel; }
			inline void resolvePosition(const Vec2i& mOffset) noexcept	{ bodyData.shape.move(mOffset); bodyData.lastResolution += mOffset; }

			inline void setPosition(const Vec2i& mPos)					{ bodyData.oldShape = getShape(); bodyData.shape.setPosition(mPos);	this->spatialInfo.invalidate(); }
			inline void setX(int mX)									{ bodyData.oldShape = getShape(); bodyData.shape.setX(mX);			this->spatialInfo.invalidate(); }
			inline void setY(int mY)									{ bodyData.oldShape = getShape(); bodyData.shape.setY(mY);			this->spatialInfo.invalidate(); }
			inline void setSize(const Vec2i& mSize)						{ bodyData.shape.setSize(mSize);									this->spatialInfo.invalidate(); }
			inline void setHalfSize(const Vec2i& mSize)					{ bodyData.shape.setHalfSize(mSize);								this->spatialInfo.invalidate(); }
			inline void setWidth(int mWidth)							{ bodyData.shape.setWidth(mWidth);									this->spatialInfo.invalidate(); }
			inline void setHeight(int mHeight)							{ bodyData.shape.setHeight(mHeight);								this->spatialInfo.invalidate(); }
			inline void setStatic(bool mStatic)							{ bodyData._static = mStatic;										this->spatialInfo.invalidate(); }
			inline void setVelocity(const Vec2f& mVel) noexcept			{ bodyData.velocity = mVel; }
			inline void setAcceleration(const Vec2f& mAccel) noexcept	{ bodyData.acceleration = mAccel; }
			inline void setUserData(void* mUserData) noexcept			{ userData = mUserData; }
			inline void setVelocityX(float mX) noexcept					{ bodyData.velocity.x = mX; }
			inline void setVelocityY(float mY) noexcept					{ bodyData.velocity.y = mY; }
			inline void setResolve(bool mResolve) noexcept				{ bodyData.resolve = mResolve; }
			inline void setMass(float mMass) noexcept					{ bodyData.setMass(mMass); }
			inline void setRestitutionX(float mX) noexcept				{ bodyData.restitution.x = mX; }
			inline void setRestitutionY(float mY) noexcept				{ bodyData.restitution.y = mY; }

			inline AABB& getShape() noexcept							{ return bodyData.shape; }
			inline AABB& getOldShape() noexcept							{ return bodyData.oldShape; }
			inline const AABB& getShape() const noexcept				{ return bodyData.shape; }
			inline const AABB& getOldShape() const noexcept				{ return bodyData.oldShape; }
			inline const Vec2i& getPosition() const noexcept			{ return getShape().getPosition(); }
			inline const Vec2f& getVelocity() const noexcept			{ return bodyData.velocity; }
			inline const Vec2i& getOldPosition() const noexcept			{ return getOldShape().getPosition(); }
			inline const Vec2f& getOldVelocity() const noexcept			{ return bodyData.oldVelocity; }
			inline const Vec2f& getAcceleration() const noexcept		{ return bodyData.acceleration; }
			inline Vec2i getSize() const noexcept						{ return getShape().getSize(); }
			inline float getMass() const noexcept						{ return isStatic() ? 0 : bodyData.mass; }
			inline float getInvMass() const noexcept					{ return isStatic() ? 0 : bodyData.invMass; }
			inline int getWidth() const noexcept						{ return getShape().getWidth(); }
			inline int getHeight() const noexcept						{ return getShape().getHeight(); }
			inline bool isStatic() const noexcept						{ return bodyData._static; }
			inline void* getUserData() const noexcept					{ return userData; }
			inline bool hasMovedLeft() const noexcept					{ return getShape().getX() < getOldShape().getX(); }
			inline bool hasMovedRight() const noexcept					{ return getShape().getX() > getOldShape().getX(); }
			inline bool hasMovedUp() const noexcept						{ return getShape().getY() < getOldShape().getY(); }
			inline bool hasMovedDown() const noexcept					{ return getShape().getY() > getOldShape().getY(); }
			inline bool getResolve() const noexcept						{ return bodyData.resolve; }
			inline const Vec2i& getLastResolution() const noexcept		{ return bodyData.lastResolution; }
			inline float getRestitutionX() const noexcept				{ return bodyData.restitution.x; }
			inline float getRestitutionY() const noexcept				{ return bodyData.restitution.y; }

			inline bool mustResolveAgainst(const Body& mBody) const noexcept { return getResolve() && !this->mustIgnoreResolution(mBody); }
	};
}

#endif

// TODO: cleanup, separate data and behavior
