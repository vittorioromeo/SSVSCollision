// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_WORLD_GROUPMANAGERD
#define SSVSC_WORLD_GROUPMANAGER

#include <map>
#include <string>

namespace ssvsc
{
	class GroupManager
	{
		private:
			int lastUid{0};
			std::map<std::string, int> uids;

		public:
			GroupManager() = default;

			inline int getUid(const std::string& mGroup)
			{
				if(uids.find(mGroup) == end(uids))
				{
					uids[mGroup] = lastUid;
					++lastUid;
				}

				return uids[mGroup];
			}
	};
}

#endif
