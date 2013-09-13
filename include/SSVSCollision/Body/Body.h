// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY
#define SSVSC_BODY

#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Body/BodyData.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/Resolver/ResolverBase.h"

namespace ssvsc
{
	class World;

	class Body : public Base
	{
		friend struct Retro;
		friend struct Impulse;

		private:
			ResolverBase& resolver;
			AABB shape, oldShape;
			bool _static, resolve{true};
			Vec2f velocity, acceleration;
			MassData massData;
			RestitutionData restitutionData;
			void* userData{nullptr};
			std::vector<Body*> bodiesToResolve;
			int spatialPaint{-1};
			Vec2i lastResolution;
			Vec2f velTransferMult, velTransferImpulse, stress, nextStress;
			float stressMult{1.f};

			inline void integrate(float mFrameTime)
			{
				velocity += acceleration * mFrameTime;
				//stress += acceleration * mFrameTime;
				shape.move(Vec2i(velocity * mFrameTime));
				ssvs::nullify(acceleration);
			}

		public:
			ssvu::Delegate<void()> onPostUpdate, onOutOfBounds;
			ssvu::Delegate<void(const ResolutionInfo&)> onResolution;

			Body(World& mWorld, bool mIsStatic, const Vec2i& mPosition, const Vec2i& mSize) : Base(mWorld), resolver(mWorld.getResolver()), shape{mPosition, mSize / 2}, oldShape{shape}, _static{mIsStatic} { spatialInfo.init(); }

			void update(float mFrameTime) override
			{
				ssvs::nullify(lastResolution);

				onPreUpdate();

				if(_static) { spatialInfo.preUpdate(); return; }
				if(outOfBounds) { onOutOfBounds(); outOfBounds = false; return; }
				oldShape = shape;
				integrate(mFrameTime);
				spatialInfo.preUpdate();

				bodiesToResolve.clear();
				spatialInfo.handleCollisions(mFrameTime);

				resolver.resolve(mFrameTime, *this, bodiesToResolve);
				if(oldShape != shape) spatialInfo.invalidate();

				spatialInfo.postUpdate(); onPostUpdate();
			}
			void handleCollision(float mFrameTime, Body* mBody) override
			{
				if(mBody == this || !mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;

				auto intersection(Utils::getMinIntersection(shape, mBody->getShape()));

				onDetection({*mBody, mBody->getUserData(), intersection, mFrameTime});
				mBody->onDetection({*this, userData, -intersection, mFrameTime});

				if(mustResolveAgainst(*mBody)) bodiesToResolve.push_back(mBody);
			}
			inline void destroy() override { spatialInfo.destroy(); Base::destroy(); }

			inline void applyForce(const Vec2f& mForce) noexcept						{ acceleration += mForce; }
			inline void applyImpulse(Vec2f mImpulse) noexcept							{ velocity += getInvMass() * mImpulse; }
			inline void applyImpulse(const Body& mBody, const Vec2f& mImpulse) noexcept	{ if(mustResolveAgainst(mBody)) applyImpulse(mImpulse); }
			inline void applyStress(const Vec2f& mStress) noexcept						{ nextStress += ssvs::getAbs(getInvMass() * mStress * stressMult); }
			inline void applyStress(const Body& mBody, const Vec2f& mStress) noexcept	{ if(mustResolveAgainst(mBody)) applyStress(mStress); }
			inline void resolvePosition(const Vec2i& mOffset) noexcept					{ shape.move(mOffset); lastResolution += mOffset; }

			inline void setPosition(const Vec2i& mPosition)				{ oldShape = shape; shape.setPosition(mPosition);	spatialInfo.invalidate(); }
			inline void setX(int mX)									{ oldShape = shape; shape.setX(mX);					spatialInfo.invalidate(); }
			inline void setY(int mY)									{ oldShape = shape; shape.setY(mY);					spatialInfo.invalidate(); }
			inline void setSize(const Vec2i& mSize)						{ shape.setSize(mSize);								spatialInfo.invalidate(); }
			inline void setHalfSize(const Vec2i& mSize)					{ shape.setHalfSize(mSize);							spatialInfo.invalidate(); }
			inline void setWidth(int mWidth)							{ shape.setWidth(mWidth);							spatialInfo.invalidate(); }
			inline void setHeight(int mHeight)							{ shape.setHeight(mHeight);							spatialInfo.invalidate(); }
			inline void setStatic(bool mStatic)							{ _static = mStatic;								spatialInfo.invalidate(); }
			inline void setVelocity(const Vec2f& mVelocity) noexcept 	{ velocity = mVelocity; }
			inline void setAcceleration(const Vec2f& mAccel) noexcept	{ acceleration = mAccel; }
			inline void setUserData(void* mUserData) noexcept			{ userData = mUserData; }
			inline void setVelocityX(float mX) noexcept					{ velocity.x = mX; }
			inline void setVelocityY(float mY) noexcept					{ velocity.y = mY; }
			inline void setResolve(bool mResolve) noexcept				{ resolve = mResolve; }
			inline void setMass(float mMass) noexcept					{ massData.setMass(mMass); }
			inline void setRestitutionX(float mRestX) noexcept			{ restitutionData.setRestitutionX(mRestX); }
			inline void setRestitutionY(float mRestY) noexcept			{ restitutionData.setRestitutionY(mRestY); }
			inline void setSpatialPaint(int mSpatialPaint) noexcept		{ spatialPaint = mSpatialPaint; }
			inline void setVelTransferMultX(float mValue) noexcept		{ velTransferMult.x = mValue; }
			inline void setVelTransferMultY(float mValue) noexcept		{ velTransferMult.y = mValue; }
			inline void setStressMult(float mValue) noexcept			{ stressMult = mValue; }

			inline BaseType getType() const noexcept override			{ return BaseType::Body; }
			inline AABB& getShape() noexcept override					{ return shape; }
			inline AABB& getOldShape() noexcept override				{ return oldShape; }
			inline const Vec2i& getPosition() const noexcept			{ return shape.getPosition(); }
			inline const Vec2f& getVelocity() const noexcept			{ return velocity; }
			inline const Vec2f& getAcceleration() const noexcept		{ return acceleration; }
			inline const Vec2i& getSize() const noexcept				{ return shape.getSize(); }
			inline float getMass() const noexcept						{ return _static ? 0 : massData.getMass(); }
			inline float getInvMass() const noexcept					{ return _static ? 0 : massData.getInvMass(); }
			inline float getRestitutionX() const noexcept				{ return restitutionData.getRestitutionX(); }
			inline float getRestitutionY() const noexcept				{ return restitutionData.getRestitutionY(); }
			inline int getWidth() const noexcept						{ return shape.getWidth(); }
			inline int getHeight() const noexcept						{ return shape.getHeight(); }
			inline bool isStatic() const noexcept						{ return _static; }
			inline void* getUserData() const noexcept					{ return userData; }
			inline bool hasMovedLeft() const noexcept					{ return shape.getX() < oldShape.getX(); }
			inline bool hasMovedRight() const noexcept					{ return shape.getX() > oldShape.getX(); }
			inline bool hasMovedUp() const noexcept						{ return shape.getY() < oldShape.getY(); }
			inline bool hasMovedDown() const noexcept					{ return shape.getY() > oldShape.getY(); }
			inline int getSpatialPaint() const noexcept					{ return spatialPaint; }
			inline bool getResolve() const noexcept						{ return resolve; }
			inline const Vec2i& getLastResolution() const noexcept		{ return lastResolution; }
			inline float getVelTransferMultX() const noexcept			{ return velTransferMult.x; }
			inline float getVelTransferMultY() const noexcept			{ return velTransferMult.y; }
			inline Vec2f& getVelTransferImpulse() noexcept				{ return velTransferImpulse; }
			inline const Vec2f& getVelTransferImpulse() const noexcept	{ return velTransferImpulse; }
			inline const Vec2f& getStress() const noexcept				{ return stress; }
			inline float getStressMult() const noexcept					{ return stressMult; }
			inline bool mustResolveAgainst(const Body& mBody) const noexcept { return resolve && !mustIgnoreResolution(mBody); }
	};
}

#endif
