// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_CELL
#define SSVSC_CELL

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
			std::vector<Body*> bodies;
			std::unordered_map<std::string, std::vector<Body*>> groupedBodies;

		public:
			Cell(int mLeft, int mRight, int mTop, int mBottom);

			void add(Body* mBody);
			void del(Body* mBody);

			inline const std::vector<Body*>& getBodies()							{ return bodies; }
			inline const std::vector<Body*>& getBodies(const std::string& mGroup)	{ return groupedBodies[mGroup]; }

			inline int getLeft() const		{ return left; }
			inline int getRight() const 	{ return right; }
			inline int getTop() const		{ return top; }
			inline int getBottom() const	{ return bottom; }
	};
}

#endif
