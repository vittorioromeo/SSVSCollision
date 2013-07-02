// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID_CELL
#define SSVSC_SPATIAL_GRID_CELL

#include <unordered_map>
#include <vector>
#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/Enums.h"

namespace ssvsc
{
	class Base;
	class Body;

	class Cell
	{
		private:
			AABB aabb;
			std::vector<Base*> bases;
			std::vector<Body*> bodies;
			std::unordered_map<int, std::vector<Body*>> groupedBodies;

			void addBody(Body* mBody);
			void delBody(Body* mBody);

		public:
			Cell(int mLeft, int mRight, int mTop, int mBottom);

			void add(Base* mBase, Type mType);
			void del(Base* mBase, Type mType);

			inline const std::vector<Base*>& getBases()					{ return bases; }
			inline const std::vector<Body*>& getBodies()				{ return bodies; }
			inline const std::vector<Body*>& getBodies(int mGroupId)	{ return groupedBodies[mGroupId]; }

			inline int getLeft() const		{ return aabb.getLeft(); }
			inline int getRight() const 	{ return aabb.getRight(); }
			inline int getTop() const		{ return aabb.getTop(); }
			inline int getBottom() const	{ return aabb.getBottom(); }
	};
}

#endif
