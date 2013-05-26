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
	using IntVector = std::vector<int>;
	using StringVector = std::vector<std::string>;

	class World;
	struct ResolverBase;

	struct MassData
	{
		float mass{1}, invMass{1};

		// Setters
		inline void setMass(float mMass) { mass = mMass; invMass = mMass == 0 ? 0 : 1.f / mMass; }

		// Getters
		inline float getMass() const	{ return mass; }
		inline float getInvMass() const	{ return invMass; }
	};

	struct GroupData
	{
		IntVector uids, uidsToCheck, uidsNoResolve;
		StringVector groups, groupsToCheck, groupsNoResolve;

		inline void addUid(int mUid)									{ uids.push_back(mUid); }
		inline void addUidToCheck(int mUid)								{ uidsToCheck.push_back(mUid); }
		inline void addUidNoResolve(int mUid)							{ uidsNoResolve.push_back(mUid); }
		inline void addGroupIds(const IntVector& mGroupUids)			{ for(const auto& uid : mGroupUids) addUid(uid); }
		inline void addGroupIdsToCheck(const IntVector& mGroupUids)		{ for(const auto& uid : mGroupUids) addUidToCheck(uid); }
		inline void addGroupIdsNoResolve(const IntVector& mGroupUids)	{ for(const auto& uid : mGroupUids) addUidNoResolve(uid); }

		inline void addGroup(const std::string& mGroup)					{ groups.push_back(mGroup); }
		inline void addGroupToCheck(const std::string& mGroup)			{ groupsToCheck.push_back(mGroup); }
		inline void addGroupNoResolve(const std::string& mGroup)		{ groupsNoResolve.push_back(mGroup); }
		inline void addGroups(const StringVector& mGroups)				{ for(const auto& g : mGroups) addGroup(g); }
		inline void addGroupsToCheck(const StringVector& mGroups)		{ for(const auto& g : mGroups) addGroupToCheck(g); }
		inline void addGroupsNoResolve(const StringVector& mGroups)		{ for(const auto& g : mGroups) addGroupNoResolve(g); }

		// Getters
		inline const IntVector& getUids()			{ return uids; }
		inline const IntVector& getUidsToCheck()	{ return uidsToCheck; }
		inline const IntVector& getUidsNoResolve()	{ return uidsNoResolve; }

		inline const StringVector& getGroups()			{ return groups; }
		inline const StringVector& getGroupsToCheck()	{ return groupsToCheck; }
		inline const StringVector& getGroupsNoResolve()	{ return groupsNoResolve; }
	};

	class Body
	{
		private:
			World& world;
			ResolverBase& resolver;
			SpatialInfoBase& spatialInfo;
			AABB shape, oldShape;
			bool _static, outOfBounds{false}, resolve{true};
			sf::Vector2f velocity, acceleration;
			MassData massData;
			GroupData groupData;
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

			void addGroups(const StringVector& mGroups);
			void addGroupsToCheck(const StringVector& mGroups);
			void addGroupsNoResolve(const StringVector& mGroups);

			void update(float mFrameTime);
			void applyForce(sf::Vector2f mForce);
			void destroy();

			// Setters
			inline void setPosition(sf::Vector2i mPosition)			{ oldShape = shape; shape.setPosition(mPosition); spatialInfo.invalidate(); }
			inline void setX(int mX)								{ oldShape = shape; shape.setX(mX); spatialInfo.invalidate(); }
			inline void setY(int mY)								{ oldShape = shape; shape.setY(mY); spatialInfo.invalidate(); }
			inline void setSize(sf::Vector2i mSize)					{ shape.setSize(mSize); spatialInfo.invalidate(); }
			inline void setWidth(int mWidth)						{ shape.setWidth(mWidth); spatialInfo.invalidate(); }
			inline void setHeight(int mHeight)						{ shape.setHeight(mHeight); spatialInfo.invalidate(); }
			inline void setVelocity(sf::Vector2f mVelocity) 		{ velocity = mVelocity; }
			inline void setAcceleration(sf::Vector2f mAcceleration)	{ acceleration = mAcceleration; }
			inline void setStatic(bool mStatic) 					{ _static = mStatic; }
			inline void setUserData(void* mUserData)				{ userData = mUserData; }
			inline void setVelocityX(float mX)						{ velocity.x = mX; }
			inline void setVelocityY(float mY)						{ velocity.y = mY; }
			inline void setOutOfBounds(bool mOutOfBounds)			{ outOfBounds = mOutOfBounds; }
			inline void setResolve(bool mResolve)					{ resolve = mResolve; }
			inline void setMass(float mMass)						{ massData.setMass(mMass); }

			// Getters
			inline SpatialInfoBase& getSpatialInfo()		{ return spatialInfo; }
			inline World& getWorld()						{ return world; }
			inline AABB& getShape()							{ return shape; }
			inline AABB& getOldShape()						{ return oldShape; }
			inline sf::Vector2i getPosition() const			{ return shape.getPosition(); }
			inline sf::Vector2f getVelocity() const			{ return velocity; }
			inline sf::Vector2f getAcceleration() const		{ return acceleration; }
			inline sf::Vector2i getSize() const				{ return shape.getSize(); }
			inline float getMass() const					{ return _static ? 0 : massData.getMass(); }
			inline float getInvMass() const					{ return _static ? 0 : massData.getInvMass(); }
			inline int getWidth() const						{ return shape.getWidth(); }
			inline int getHeight() const					{ return shape.getHeight(); }
			inline bool isStatic() const					{ return _static; }
			inline void* getUserData() const				{ return userData; }
			inline bool hasMovedLeft() const				{ return shape.getX() < oldShape.getX(); }
			inline bool hasMovedRight() const				{ return shape.getX() > oldShape.getX(); }
			inline bool hasMovedUp() const					{ return shape.getY() < oldShape.getY(); }
			inline bool hasMovedDown() const				{ return shape.getY() > oldShape.getY(); }
			inline const IntVector& getGroupUids()			{ return groupData.getUids(); }
			inline const IntVector& getGroupUidsToCheck()	{ return groupData.getUidsToCheck(); }
			inline const IntVector& getGroupUidsNoResolve()	{ return groupData.getUidsNoResolve(); }
			inline const StringVector& getGroups()			{ return groupData.getGroups(); }
			inline const StringVector& getGroupsToCheck()	{ return groupData.getGroupsToCheck(); }
			inline const StringVector& getGroupsNoResolve()	{ return groupData.getGroupsNoResolve(); }
	};
}

#endif
