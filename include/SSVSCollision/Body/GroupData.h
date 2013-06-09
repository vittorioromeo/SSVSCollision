// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_GROUPDATA
#define SSVSC_BODY_GROUPDATA

#include <vector>
#include <string>

namespace ssvsc
{
	struct GroupData
	{
		std::vector<int> uids, uidsToCheck, uidsNoResolve;
		std::vector<std::string> groups, groupsToCheck, groupsNoResolve;

		inline void addUid(int mUid)												{ uids.push_back(mUid); }
		inline void addUidToCheck(int mUid)											{ uidsToCheck.push_back(mUid); }
		inline void addUidNoResolve(int mUid)										{ uidsNoResolve.push_back(mUid); }
		inline void addGroupIds(const std::vector<int>& mGroupUids)					{ for(const auto& uid : mGroupUids) addUid(uid); }
		inline void addGroupIdsToCheck(const std::vector<int>& mGroupUids)			{ for(const auto& uid : mGroupUids) addUidToCheck(uid); }
		inline void addGroupIdsNoResolve(const std::vector<int>& mGroupUids)		{ for(const auto& uid : mGroupUids) addUidNoResolve(uid); }

		inline void addGroup(const std::string& mGroup)								{ groups.push_back(mGroup); }
		inline void addGroupToCheck(const std::string& mGroup)						{ groupsToCheck.push_back(mGroup); }
		inline void addGroupNoResolve(const std::string& mGroup)					{ groupsNoResolve.push_back(mGroup); }
		inline void addGroups(const std::vector<std::string>& mGroups)				{ for(const auto& g : mGroups) addGroup(g); }
		inline void addGroupsToCheck(const std::vector<std::string>& mGroups)		{ for(const auto& g : mGroups) addGroupToCheck(g); }
		inline void addGroupsNoResolve(const std::vector<std::string>& mGroups)		{ for(const auto& g : mGroups) addGroupNoResolve(g); }

		// Getters
		inline const std::vector<int>& getUids()					{ return uids; }
		inline const std::vector<int>& getUidsToCheck()				{ return uidsToCheck; }
		inline const std::vector<int>& getUidsNoResolve()			{ return uidsNoResolve; }

		inline const std::vector<std::string>& getGroups()			{ return groups; }
		inline const std::vector<std::string>& getGroupsToCheck()	{ return groupsToCheck; }
		inline const std::vector<std::string>& getGroupsNoResolve()	{ return groupsNoResolve; }
	};
}

#endif
