// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_GROUPABLE
#define SSVSC_BODY_GROUPABLE

#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	class Groupable
	{
		private:
			Bitset groups, groupsToCheck, groupsNoResolve;

		public:
			inline void addGroups(Group mGroup)	noexcept													{ groups[mGroup] = true; }
			inline void delGroups(Group mGroup)	noexcept													{ groups[mGroup] = false; }
			template<typename... TGroups> inline void addGroups(Group mGroup, TGroups... mGroups) noexcept	{ groups[mGroup] = true; addGroups(mGroups...); }
			template<typename... TGroups> inline void delGroups(Group mGroup, TGroups... mGroups) noexcept	{ groups[mGroup] = false; delGroups(mGroups...); }

			inline void addGroupsToCheck(Group mGroup) noexcept														{ groupsToCheck[mGroup] = true; }
			inline void delGroupsToCheck(Group mGroup) noexcept	 													{ groupsToCheck[mGroup] = false; }
			template<typename... TGroups> inline void addGroupsToCheck(Group mGroup, TGroups... mGroups) noexcept	{ groupsToCheck[mGroup] = true; addGroupsToCheck(mGroups...); }
			template<typename... TGroups> inline void delGroupsToCheck(Group mGroup, TGroups... mGroups) noexcept	{ groupsToCheck[mGroup] = false; delGroupsToCheck(mGroups...); }

			inline void addGroupsNoResolve(Group mGroup) noexcept													{ groupsNoResolve[mGroup] = true; }
			inline void delGroupsNoResolve(Group mGroup) noexcept													{ groupsNoResolve[mGroup] = false; }
			template<typename... TGroups> inline void addGroupsNoResolve(Group mGroup, TGroups... mGroups) noexcept	{ groupsNoResolve[mGroup] = true; addGroupsNoResolve(mGroups...); }
			template<typename... TGroups> inline void delGroupsNoResolve(Group mGroup, TGroups... mGroups) noexcept	{ groupsNoResolve[mGroup] = false; delGroupsNoResolve(mGroups...); }

			inline void addGroup(Group mGroup) noexcept					{ addGroups(mGroup); }
			inline void addGroupToCheck(Group mGroup) noexcept			{ addGroupsToCheck(mGroup); }
			inline void addGroupNoResolve(Group mGroup)	noexcept 		{ addGroupsNoResolve(mGroup); }

			inline void delGroup(Group mGroup) noexcept					{ delGroups(mGroup); }
			inline void delGroupToCheck(Group mGroup) noexcept			{ delGroupsToCheck(mGroup); }
			inline void delGroupNoResolve(Group mGroup) noexcept		{ delGroupNoResolve(mGroup); }

			inline void clearGroups() noexcept							{ groups.reset(); }
			inline void clearGroupsToCheck() noexcept					{ groupsToCheck.reset(); }
			inline void clearGroupsNoResolve() noexcept					{ groupsNoResolve.reset(); }

			constexpr inline bool hasGroup(Group mGroup) const noexcept				{ return groups[mGroup]; }
			constexpr inline bool hasGroupToCheck(Group mGroup) const noexcept		{ return groupsToCheck[mGroup]; }
			constexpr inline bool hasGroupNoResolve(Group mGroup) const noexcept	{ return groupsNoResolve[mGroup]; }

			inline bool hasAnyGroup(const Bitset& mGroups) const noexcept			{ return (groups & mGroups).any(); }
			inline bool hasAnyGroupToCheck(const Bitset& mGroups) const noexcept	{ return (groupsToCheck & mGroups).any(); }
			inline bool hasAnyGroupNoResolve(const Bitset& mGroups) const noexcept	{ return (groupsNoResolve & mGroups).any(); }

			inline bool hasAllGroups(const Bitset& mGroups) const noexcept			{ return (groups & mGroups).all(); }
			inline bool hasAllGroupsToCheck(const Bitset& mGroups) const noexcept	{ return (groupsToCheck & mGroups).all(); }
			inline bool hasAllGroupsNoResolve(const Bitset& mGroups) const noexcept	{ return (groupsNoResolve & mGroups).all(); }

			inline const Bitset& getGroups() const noexcept				{ return groups; }
			inline const Bitset& getGroupsToCheck()	const noexcept		{ return groupsToCheck; }
			inline const Bitset& getGroupsNoResolve() const noexcept	{ return groupsNoResolve; }
	};
}

#endif
