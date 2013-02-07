// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef CELL_H
#define CELL_H

#include <unordered_map>
#include <vector>
#include <string>

namespace ssvsc
{
	class Body;

	class Cell
	{
		private:
			int left, right, top, bottom;
			std::unordered_map<std::string, std::vector<Body*>> bodies;

		public:
			Cell(int mLeft, int mRight, int mTop, int mBottom);			

			void add(Body* mBody);
			void del(Body* mBody);
			bool hasGroup(const std::string& mGroup);

			// Properties
			std::vector<Body*>* getQuery(const std::string& mGroup);
			int getLeft() const;
			int getRight() const;
			int getTop() const;
			int getBottom() const;
	};
}

#endif // CELL_H
