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

			inline void integrate(float mFrameTime)
			{
				velocity += acceleration * mFrameTime;
				shape.move(Vec2i(velocity * mFrameTime));
				ssvs::nullify(acceleration);
			}

		public:
			ssvu::Delegate<void()> onPostUpdate, onOutOfBounds;
			ssvu::Delegate<void(const ResolutionInfo&)> onResolution;

			Body(World& mWorld, bool mIsStatic, const Vec2i& mPosition, const Vec2i& mSize) : Base(mWorld), resolver(mWorld.getResolver()), shape{mPosition, mSize / 2}, oldShape{shape}, _static{mIsStatic} { spatialInfo.init(); }
			~Body() { spatialInfo.destroy(); }

			void update(float mFrameTime) override
			{
				onPreUpdate();

				if(_static) { spatialInfo.preUpdate(); return; }
				if(outOfBounds) { onOutOfBounds(); outOfBounds = false; return; }
				oldShape = shape;
				integrate(mFrameTime);
				spatialInfo.preUpdate();

				bodiesToResolve.clear();
				spatialInfo.handleCollisions(mFrameTime);

				resolver.resolve(*this, bodiesToResolve);
				if(oldShape != shape) spatialInfo.invalidate();

				spatialInfo.postUpdate(); onPostUpdate();
			}
			void handleCollision(float mFrameTime, Body* mBody) override
			{
				if(mBody == this || !mustCheck(*mBody) || !shape.isOverlapping(mBody->getShape())) return;

				auto intersection(Utils::getMinIntersection(shape, mBody->getShape()));

				onDetection({*mBody, mBody->getUserData(), intersection, mFrameTime});
				mBody->onDetection({*this, userData, -intersection, mFrameTime});

				if(resolve && !mustIgnoreResolution(*mBody)) bodiesToResolve.push_back(mBody);
			}
			inline void destroy() override { Base::destroy(); }

			inline void applyForce(const Vec2f& mForce)			{ acceleration += mForce; }
			inline void applyImpulse(const Vec2f& mImpulse)		{ velocity += getInvMass() * mImpulse; }

			inline void setPosition(const Vec2i& mPosition)		{ oldShape = shape; shape.setPosition(mPosition); spatialInfo.invalidate(); }
			inline void setX(int mX)							{ oldShape = shape; shape.setX(mX); spatialInfo.invalidate(); }
			inline void setY(int mY)							{ oldShape = shape; shape.setY(mY); spatialInfo.invalidate(); }
			inline void setSize(const Vec2i& mSize)				{ shape.setSize(mSize); spatialInfo.invalidate(); }
			inline void setHalfSize(const Vec2i& mSize)			{ shape.setHalfSize(mSize); spatialInfo.invalidate(); }
			inline void setWidth(int mWidth)					{ shape.setWidth(mWidth); spatialInfo.invalidate(); }
			inline void setHeight(int mHeight)					{ shape.setHeight(mHeight); spatialInfo.invalidate(); }
			inline void setVelocity(const Vec2f& mVelocity) 	{ velocity = mVelocity; }
			inline void setAcceleration(const Vec2f& mAccel)	{ acceleration = mAccel; }
			inline void setStatic(bool mStatic)					{ _static = mStatic; spatialInfo.invalidate(); }
			inline void setUserData(void* mUserData)			{ userData = mUserData; }
			inline void setVelocityX(float mX)					{ velocity.x = mX; }
			inline void setVelocityY(float mY)					{ velocity.y = mY; }
			inline void setResolve(bool mResolve)				{ resolve = mResolve; }
			inline void setMass(float mMass)					{ massData.setMass(mMass); }
			inline void setRestitutionX(float mRestX)			{ restitutionData.setRestitutionX(mRestX); }
			inline void setRestitutionY(float mRestY)			{ restitutionData.setRestitutionY(mRestY); }
			inline void setSpatialPaint(int mSpatialPaint)		{ spatialPaint = mSpatialPaint; }

			inline BaseType getType() override			{ return BaseType::Body; }
			inline AABB& getShape() override			{ return shape; }
			inline AABB& getOldShape() override			{ return oldShape; }
			inline const Vec2i& getPosition() const		{ return shape.getPosition(); }
			inline const Vec2f& getVelocity() const		{ return velocity; }
			inline const Vec2f& getAcceleration() const	{ return acceleration; }
			inline const Vec2i& getSize() const			{ return shape.getSize(); }
			inline float getMass() const				{ return _static ? 0 : massData.getMass(); }
			inline float getInvMass() const				{ return _static ? 0 : massData.getInvMass(); }
			inline float getRestitutionX() const		{ return restitutionData.getRestitutionX(); }
			inline float getRestitutionY() const		{ return restitutionData.getRestitutionY(); }
			inline int getWidth() const					{ return shape.getWidth(); }
			inline int getHeight() const				{ return shape.getHeight(); }
			inline bool isStatic() const				{ return _static; }
			inline void* getUserData() const			{ return userData; }
			inline bool hasMovedLeft() const			{ return shape.getX() < oldShape.getX(); }
			inline bool hasMovedRight() const			{ return shape.getX() > oldShape.getX(); }
			inline bool hasMovedUp() const				{ return shape.getY() < oldShape.getY(); }
			inline bool hasMovedDown() const			{ return shape.getY() > oldShape.getY(); }
			inline int getSpatialPaint() const			{ return spatialPaint; }
	};
}

#endif
