// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY
#define SSVSC_BODY

#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Body/MassData.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Base.h"

namespace ssvsc
{
	class World;
	struct ResolverBase;

	class Body : public Base
	{
		friend class GridInfo;

		private:
			ResolverBase& resolver;
			AABB shape, oldShape;
			bool _static, resolve{true};
			Vec2f velocity, acceleration;
			MassData massData;
			void* userData{nullptr};
			std::vector<Body*> bodiesToResolve;
			int paint{-1};

			void integrate(float mFrameTime);

		public:

			void handleCollision(float mFrameTime, Body* mBody) override;

			ssvu::Delegate<void> onPreUpdate;
			ssvu::Delegate<void> onPostUpdate;
			ssvu::Delegate<void, DetectionInfo> onDetection;
			ssvu::Delegate<void, ResolutionInfo> onResolution;
			ssvu::Delegate<void> onOutOfBounds;

			Body(World& mWorld, bool mIsStatic, Vec2i mPosition, Vec2i mSize);
			~Body();

			inline Type getType() override { return Type::Body; }
			void update(float mFrameTime) override;
			void destroy() override;

			inline void applyForce(Vec2f mForce)	{ if(!_static) acceleration += mForce; }
			void applyImpulse(Vec2f mImpulse)		{ velocity += getInvMass() * mImpulse; }

			// Setters
			inline void setPosition(Vec2i mPosition)			{ oldShape = shape; shape.setPosition(mPosition); spatialInfo.invalidate(); }
			inline void setX(int mX)							{ oldShape = shape; shape.setX(mX); spatialInfo.invalidate(); }
			inline void setY(int mY)							{ oldShape = shape; shape.setY(mY); spatialInfo.invalidate(); }
			inline void setSize(Vec2i mSize)					{ shape.setSize(mSize); spatialInfo.invalidate(); }
			inline void setWidth(int mWidth)					{ shape.setWidth(mWidth); spatialInfo.invalidate(); }
			inline void setHeight(int mHeight)					{ shape.setHeight(mHeight); spatialInfo.invalidate(); }
			inline void setVelocity(Vec2f mVelocity) 			{ velocity = mVelocity; }
			inline void setAcceleration(Vec2f mAcceleration)	{ acceleration = mAcceleration; }
			inline void setStatic(bool mStatic) 				{ _static = mStatic; }
			inline void setUserData(void* mUserData)			{ userData = mUserData; }
			inline void setVelocityX(float mX)					{ velocity.x = mX; }
			inline void setVelocityY(float mY)					{ velocity.y = mY; }
			inline void setResolve(bool mResolve)				{ resolve = mResolve; }
			inline void setMass(float mMass)					{ massData.setMass(mMass); }

			// Getters
			inline AABB& getShape() override		{ return shape; }
			inline AABB& getOldShape() override		{ return oldShape; }
			inline Vec2i getPosition() const		{ return shape.getPosition(); }
			inline Vec2f getVelocity() const		{ return velocity; }
			inline Vec2f getAcceleration() const	{ return acceleration; }
			inline Vec2i getSize() const			{ return shape.getSize(); }
			inline float getMass() const			{ return _static ? 0 : massData.getMass(); }
			inline float getInvMass() const			{ return _static ? 0 : massData.getInvMass(); }
			inline int getWidth() const				{ return shape.getWidth(); }
			inline int getHeight() const			{ return shape.getHeight(); }
			inline bool isStatic() const			{ return _static; }
			inline void* getUserData() const		{ return userData; }
			inline bool hasMovedLeft() const		{ return shape.getX() < oldShape.getX(); }
			inline bool hasMovedRight() const		{ return shape.getX() > oldShape.getX(); }
			inline bool hasMovedUp() const			{ return shape.getY() < oldShape.getY(); }
			inline bool hasMovedDown() const		{ return shape.getY() > oldShape.getY(); }
	};
}

#endif
