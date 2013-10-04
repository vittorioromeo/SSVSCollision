// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY
#define SSVSC_BODY

#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Body/BodyData.h"
#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Utils/Utils.h"

namespace ssvsc
{
	template<typename TW> class Body : public Base<TW>
	{
		public:
			using BaseType = Base<TW>;
			using BodyType = Body<TW>;
			using ResolverType = typename TW::ResolverType;
			using ResolutionInfoType = typename TW::ResolutionInfoType;
			friend ResolverType;

		private:
			ResolverType& resolver;
			AABB shape, oldShape;
			bool _static, resolve{true};
			Vec2f velocity, oldVelocity, acceleration;
			MassData massData;
			RestitutionData restitutionData;
			void* userData{nullptr};
			std::vector<BodyType*> toResolve;
			Vec2i lastResolution;

			// TODO: ResolverInfo? definitely.
			Vec2f velTransferMult, velTransferImpulse, stress, nextStress;
			float stressMult{1.f}, stressPropagationMult{0.1f};

			bool mustInit{true};

			inline void integrate(float mFT)
			{
				velocity += acceleration * mFT;
				shape.move(Vec2i(velocity * mFT));
				ssvs::nullify(acceleration);
			}

		public:
			ssvu::Delegate<void()> onPostUpdate, onOutOfBounds;
			ssvu::Delegate<void(const ResolutionInfoType&)> onResolution;

			inline Body(TW& mWorld, bool mIsStatic, const Vec2i& mPos, const Vec2i& mSize) noexcept : BaseType(mWorld), resolver(mWorld.resolver), shape{mPos, mSize / 2}, oldShape{shape}, _static{mIsStatic} { }
			inline ~Body() noexcept { destroy(); }

			void update(float mFT) override
			{
				if(mustInit) { this->spatialInfo.init(); mustInit = false; }

				ssvs::nullify(lastResolution);

				this->onPreUpdate();

				if(_static) { this->spatialInfo.preUpdate(); return; }
				if(this->outOfBounds) { onOutOfBounds(); this->outOfBounds = false; return; }
				oldShape = shape;
				oldVelocity = velocity;
				integrate(mFT);
				this->spatialInfo.preUpdate();

				toResolve.clear();
				this->spatialInfo.handleCollisions(mFT);

				resolver.resolve(*this, toResolve);
				if(oldShape != shape) this->spatialInfo.invalidate();

				this->spatialInfo.postUpdate(); onPostUpdate();
			}
			void handleCollision(float mFT, BodyType* mBody) override
			{
				if(mBody == this || !this->mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;

				auto intersection(Utils::getMinIntersection(shape, mBody->getShape()));

				this->onDetection({*mBody, mBody->getUserData(), intersection, mFT});
				mBody->onDetection({*this, userData, -intersection, mFT});

				if(mustResolveAgainst(*mBody)) toResolve.push_back(mBody);
			}
			inline void destroy() override { this->spatialInfo.destroy(); BaseType::destroy(); }

			inline void applyForce(const Vec2f& mForce) noexcept { acceleration += mForce; }
			inline void applyImpulse(const Vec2f& mImpulse) noexcept
			{
				velocity.x += getInvMass() * (mImpulse.x / (1.f + (stress.y * stressPropagationMult)));
				velocity.y += getInvMass() * (mImpulse.y / (1.f + (stress.x * stressPropagationMult)));
			}
			inline void applyImpulse(const Body& mBody, const Vec2f& mImpulse) noexcept	{ if(mustResolveAgainst(mBody)) applyImpulse(mImpulse); }
			inline void applyStress(const Vec2f& mStress) noexcept						{ nextStress += ssvs::getAbs(getInvMass() * mStress * stressMult); }
			inline void applyStress(const Body& mBody, const Vec2f& mStress) noexcept	{ if(mustResolveAgainst(mBody)) applyStress(mStress); }
			inline void resolvePosition(const Vec2i& mOffset) noexcept					{ shape.move(mOffset); lastResolution += mOffset; }

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
			inline void setRestitutionX(float mRestX) noexcept			{ restitutionData.setRestitutionX(mRestX); }
			inline void setRestitutionY(float mRestY) noexcept			{ restitutionData.setRestitutionY(mRestY); }
			inline void setVelTransferMultX(float mValue) noexcept		{ velTransferMult.x = mValue; }
			inline void setVelTransferMultY(float mValue) noexcept		{ velTransferMult.y = mValue; }
			inline void setStressMult(float mValue) noexcept			{ stressMult = mValue; }
			inline void setStressPropagationMult(float mValue) noexcept	{ stressPropagationMult = mValue; }

			inline BBType getType() const noexcept override							{ return BBType::Body; }
			inline AABB& getShape() noexcept override								{ return shape; }
			inline AABB& getOldShape() noexcept override							{ return oldShape; }
			inline const Vec2i& getPosition() const noexcept						{ return shape.getPosition(); }
			inline const Vec2f& getVelocity() const noexcept						{ return velocity; }
			inline const Vec2i& getOldPosition() const noexcept						{ return oldShape.getPosition(); }
			inline const Vec2f& getOldVelocity() const noexcept						{ return oldVelocity; }
			inline const Vec2f& getAcceleration() const noexcept					{ return acceleration; }
			inline const Vec2i& getSize() const noexcept							{ return shape.getSize(); }
			inline float getMass() const noexcept									{ return _static ? 0 : massData.getMass(); }
			inline float getInvMass() const noexcept								{ return _static ? 0 : massData.getInvMass(); }
			inline float getRestitutionX() const noexcept							{ return restitutionData.getRestitutionX(); }
			inline float getRestitutionY() const noexcept							{ return restitutionData.getRestitutionY(); }
			inline int getWidth() const noexcept									{ return shape.getWidth(); }
			inline int getHeight() const noexcept									{ return shape.getHeight(); }
			inline bool isStatic() const noexcept									{ return _static; }
			inline void* getUserData() const noexcept								{ return userData; }
			inline bool hasMovedLeft() const noexcept								{ return shape.getX() < oldShape.getX(); }
			inline bool hasMovedRight() const noexcept								{ return shape.getX() > oldShape.getX(); }
			inline bool hasMovedUp() const noexcept									{ return shape.getY() < oldShape.getY(); }
			inline bool hasMovedDown() const noexcept								{ return shape.getY() > oldShape.getY(); }
			inline bool getResolve() const noexcept									{ return resolve; }
			inline const Vec2i& getLastResolution() const noexcept					{ return lastResolution; }
			inline float getVelTransferMultX() const noexcept						{ return velTransferMult.x; }
			inline float getVelTransferMultY() const noexcept						{ return velTransferMult.y; }
			inline Vec2f& getVelTransferImpulse() noexcept							{ return velTransferImpulse; }
			inline const Vec2f& getVelTransferImpulse() const noexcept				{ return velTransferImpulse; }
			inline const Vec2f& getStress() const noexcept							{ return stress; }
			inline float getStressMult() const noexcept								{ return stressMult; }
			inline float getStressPropagationMult() const noexcept					{ return stressPropagationMult; }
			inline bool mustResolveAgainst(const BodyType& mBody) const noexcept	{ return resolve && !this->mustIgnoreResolution(mBody); }
	};
}

#endif
