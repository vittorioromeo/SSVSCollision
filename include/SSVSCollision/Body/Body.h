// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY
#define SSVSC_BODY

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"

namespace ssvsc
{
	class World;
	class ResolverBase;

	class Body
	{
		private:
			World& world;
			ResolverBase& resolver;
			SpatialInfoBase& spatialInfo;
			AABB shape, oldShape;
			bool _static, outOfBounds{false}, resolve{true};
			sf::Vector2f velocity, acceleration;
			float mass{1}, invMass{1};
			std::vector<std::string> groups, groupsToCheck, groupsNoResolve;
			void* userData{nullptr};
			std::vector<Body*> bodiesToResolve;

			void integrate(float mFrameTime);

		public:
			ssvu::Delegate<void> onPreUpdate;
			ssvu::Delegate<void> onPostUpdate;
			ssvu::Delegate<void, DetectionInfo> onDetection;
			ssvu::Delegate<void, ResolutionInfo> onResolution;
			ssvu::Delegate<void> onOutOfBounds;

			Body(World& mWorld, bool mIsStatic, sf::Vector2i mPosition, sf::Vector2i mSize);
			~Body();

			void addGroups(const std::vector<std::string>& mGroups);
			void addGroupsToCheck(const std::vector<std::string>& mGroups);
			void addGroupsNoResolve(const std::vector<std::string>& mGroups);
			void update(float mFrameTime);
			void applyForce(sf::Vector2f mForce);
			void destroy();

			// Setters
			void setPosition(sf::Vector2i mPosition);
			void setX(int mX);
			void setY(int mY);
			void setSize(sf::Vector2i mSize);
			void setWidth(int mWidth);
			void setHeight(int mHeight);
			inline void setVelocity(sf::Vector2f mVelocity) 		{ velocity = mVelocity; }
			inline void setAcceleration(sf::Vector2f mAcceleration)	{ acceleration = mAcceleration; }
			inline void setStatic(bool mStatic) 					{ _static = mStatic; }
			inline void setUserData(void* mUserData)				{ userData = mUserData; }
			inline void setVelocityX(float mX)						{ velocity.x = mX; }
			inline void setVelocityY(float mY)						{ velocity.y = mY; }
			inline void setOutOfBounds(bool mOutOfBounds)			{ outOfBounds = mOutOfBounds; }
			inline void setResolve(bool mResolve)					{ resolve = mResolve; }
			inline void setMass(float mMass)						{ mass = mMass; invMass = mMass == 0 ? 0 : 1.f / mMass; }

			// Getters
			inline SpatialInfoBase& getSpatialInfo()	{ return spatialInfo; }
			inline World& getWorld()					{ return world; }
			inline AABB& getShape()						{ return shape; }
			inline AABB& getOldShape() 					{ return oldShape; }
			inline sf::Vector2i getPosition() const		{ return shape.getPosition(); }
			inline sf::Vector2f getVelocity() const		{ return velocity; }
			inline sf::Vector2f getAcceleration() const	{ return acceleration; }
			inline sf::Vector2i getSize() const			{ return shape.getSize(); }
			inline float getMass() const				{ return _static ? 0 : mass; }
			inline float getInvMass() const				{ return _static ? 0 : invMass; }
			inline int getWidth() const					{ return shape.getWidth(); }
			inline int getHeight() const				{ return shape.getHeight(); }
			inline bool isStatic() const				{ return _static; }
			inline void* getUserData() const			{ return userData; }
			inline bool hasMovedLeft() const			{ return shape.getX() < oldShape.getX(); }
			inline bool hasMovedRight() const			{ return shape.getX() > oldShape.getX(); }
			inline bool hasMovedUp() const				{ return shape.getY() < oldShape.getY(); }
			inline bool hasMovedDown() const			{ return shape.getY() > oldShape.getY(); }
			inline const std::vector<std::string>& getGroups()			{ return groups; }
			inline const std::vector<std::string>& getGroupsToCheck()	{ return groupsToCheck; }
			inline const std::vector<std::string>& getGroupsNoResolve()	{ return groupsNoResolve; }
	};
}

#endif
