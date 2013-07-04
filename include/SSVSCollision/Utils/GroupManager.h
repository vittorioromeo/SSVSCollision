// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS_GROUPMANAGER
#define SSVSC_UTILS_GROUPMANAGER

#include <unordered_map>
#include <string>
#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	class GroupManager
	{
		private:
			Group lastGroup{0};
			std::unordered_map<std::string, Group> groups;

		public:
			GroupManager() = default;

			inline Group get(const std::string& mLabel)
			{
				if(groups.find(mLabel) == end(groups))
				{
					groups[mLabel] = lastGroup;
					++lastGroup;
				}

				return groups[mLabel];
			}
	};
}

#endif
