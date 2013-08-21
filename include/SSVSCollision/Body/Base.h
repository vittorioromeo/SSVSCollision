// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/World/World.h"
#include "SSVSCollision/Spatial/SpatialBase.h"
#include "SSVSCollision/Body/GroupData.h"

namespace ssvsc
{
	class AABB;
	class Body;
	class SpatialInfoBase;

	class Base : public ssvu::MemoryManageable
	{
		protected:
			World& world;
			SpatialInfoBase& spatialInfo;
			GroupData groupData;
			bool outOfBounds{false};

			Base(World& mWorld) : world(mWorld), spatialInfo(world.getSpatial().createSpatialInfo(*this)) { }

		public:
			virtual ~Base() { }

			virtual	void update(float mFrameTime) = 0;
			virtual void handleCollision(float mFrameTime, Body* mBody) = 0;
			inline virtual void destroy() { world.del(*this); }

			inline void setOutOfBounds(bool mOutOfBounds)	{ outOfBounds = mOutOfBounds; }

			virtual AABB& getShape() = 0;
			virtual AABB& getOldShape() = 0;
			virtual BaseType getType() = 0;
			inline SpatialInfoBase& getSpatialInfo()	{ return spatialInfo; }
			inline World& getWorld() const				{ return world; }

			inline void addGroup(Group mGroup)					{ groupData.addGroup(mGroup); }
			inline void addGroupToCheck(Group mGroup)			{ groupData.addGroupToCheck(mGroup); }
			inline void addGroupNoResolve(Group mGroup)			{ groupData.addGroupNoResolve(mGroup); }
			inline void delGroup(Group mGroup)					{ groupData.delGroup(mGroup); }
			inline void delGroupToCheck(Group mGroup)			{ groupData.delGroupToCheck(mGroup); }
			inline void delGroupNoResolve(Group mGroup)			{ groupData.delGroupNoResolve(mGroup); }
			inline void clearGroups()							{ groupData.clearGroups(); }
			inline void clearGroupsToCheck()					{ groupData.clearGroupsToCheck(); }
			inline void clearGroupsNoResolve()					{ groupData.clearGroupsNoResolve(); }
			inline bool hasGroup(Group mGroup) const			{ return groupData.hasGroup(mGroup); }
			inline bool hasGroupToCheck(Group mGroup) const		{ return groupData.hasGroupToCheck(mGroup); }
			inline bool hasGroupNoResolve(Group mGroup) const	{ return groupData.hasGroupNoResolve(mGroup); }
			inline const Bitset& getGroups() const				{ return groupData.getGroups(); }
			inline const Bitset& getGroupsToCheck()	const		{ return groupData.getGroupsToCheck(); }
			inline const Bitset& getGroupsNoResolve() const		{ return groupData.getGroupsNoResolve(); }

			inline bool hasAnyGroup(const Bitset& mGroups) const		{ return (groupData.getGroups() & mGroups).any(); }
			inline bool mustCheck(const Base& mBase) const				{ return mBase.hasAnyGroup(groupData.getGroupsToCheck()); }
			inline bool mustIgnoreResolution(const Base& mBase) const	{ return mBase.hasAnyGroup(groupData.getGroupsNoResolve()); }
	};
}

#endif
