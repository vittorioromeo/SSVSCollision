// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_GROUPDATA
#define SSVSC_BODY_GROUPDATA

#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	class GroupData
	{
		private:
			Bitset groups, groupsToCheck, groupsNoResolve;

		public:
			inline void addGroup(Group mGroup)					{ groups.set(mGroup); }
			inline void addGroupToCheck(Group mGroup)			{ groupsToCheck.set(mGroup); }
			inline void addGroupNoResolve(Group mGroup)			{ groupsNoResolve.set(mGroup); }

			inline void delGroup(Group mGroup)					{ groups.set(mGroup, false); }
			inline void delGroupToCheck(Group mGroup)			{ groupsToCheck.set(mGroup, false); }
			inline void delGroupNoResolve(Group mGroup)			{ groupsNoResolve.set(mGroup, false); }

			inline void clearGroups()							{ groups.reset(); }
			inline void clearGroupsToCheck()					{ groupsToCheck.reset(); }
			inline void clearGroupsNoResolve()					{ groupsNoResolve.reset(); }

			inline bool hasGroup(Group mGroup) const			{ return groups.test(mGroup); }
			inline bool hasGroupToCheck(Group mGroup) const		{ return groupsToCheck.test(mGroup); }
			inline bool hasGroupNoResolve(Group mGroup) const	{ return groupsNoResolve.test(mGroup); }

			inline const Bitset& getGroups() const				{ return groups; }
			inline const Bitset& getGroupsToCheck()	const		{ return groupsToCheck; }
			inline const Bitset& getGroupsNoResolve() const		{ return groupsNoResolve; }
	};
}

#endif
