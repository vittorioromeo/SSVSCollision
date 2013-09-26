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
	template<typename> class BodyType;

	template<typename TS> class BaseType : public ssvu::MemoryManageable
	{
		protected:
			World<TS>& world;

		public:
			using WT = typename std::remove_reference<decltype(world)>::type;
			using Base = typename WT::Base;
			using Body = typename WT::Body;
			using Sensor = typename WT::Sensor;
			using Resolver = typename WT::Resolver;
			using DI = typename WT::DI;
			using RI = typename WT::RI;
			using SI = typename WT::SI;

		protected:
			SI spatialInfo;
			GroupData groupData;
			bool outOfBounds{false};

			BaseType(World<TS>& mWorld) : world(mWorld), spatialInfo(world.getSpatial(), *this) { }

		public:
			ssvu::Delegate<void()> onPreUpdate;
			ssvu::Delegate<void(const DI&)> onDetection;

			inline virtual ~BaseType() { }

			virtual	void update(float mFT) = 0;
			virtual void handleCollision(float mFT, Body* mBody) = 0;
			inline virtual void destroy() { world.del(*this); }

			inline void setOutOfBounds(bool mOutOfBounds) noexcept { outOfBounds = mOutOfBounds; }

			virtual AABB& getShape() noexcept = 0;
			virtual AABB& getOldShape() noexcept = 0;
			virtual BType getType() const noexcept = 0;
			inline SI& getSpatialInfo() noexcept	{ return spatialInfo; }
			inline World<TS>& getWorld() const noexcept				{ return world; }

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
