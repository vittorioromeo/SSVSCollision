// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BASE
#define SSVSC_BASE

#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/World/World.h"
#include "SSVSCollision/Body/GroupData.h"

namespace ssvsc
{
	class AABB;
	class Body;
	class SpatialInfoBase;

	class Base
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
			virtual AABB& getShape() = 0;
			virtual AABB& getOldShape() = 0;
			virtual Type getType() = 0;
			virtual void destroy() { world.delBase(this); }

			inline SpatialInfoBase& getSpatialInfo()		{ return spatialInfo; }
			inline void setOutOfBounds(bool mOutOfBounds)	{ outOfBounds = mOutOfBounds; }

			// GroupData shortcuts
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

			// GroupData + GroupManager shortcuts
			void addGroups(const std::vector<std::string>& mLabels)				{ for(const auto& l : mLabels) addGroup(world.getGroup(l)); }
			void addGroupsToCheck(const std::vector<std::string>& mLabels)		{ for(const auto& l : mLabels) addGroupToCheck(world.getGroup(l)); }
			void addGroupsNoResolve(const std::vector<std::string>& mLabels)	{ for(const auto& l : mLabels) addGroupNoResolve(world.getGroup(l)); }
	};
}

#endif
