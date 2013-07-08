// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID_CELL
#define SSVSC_SPATIAL_GRID_CELL

#include <vector>
#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Body/Body.h"

namespace ssvsc
{
	class Base;
	class Body;

	class Cell
	{
		private:
			AABB aabb;
			std::vector<Body*> bodies;

			inline void addBody(Body* mBody) { bodies.push_back(mBody); }
			inline void delBody(Body* mBody) { ssvu::eraseRemove(bodies, mBody); }

		public:
			Cell(const AABB& mAABB) : aabb{mAABB} { }

			inline void add(Base* mBase) { if(mBase->getType() == Type::Body) addBody(static_cast<Body*>(mBase)); }
			inline void del(Base* mBase) { if(mBase->getType() == Type::Body) delBody(static_cast<Body*>(mBase)); }

			inline const std::vector<Body*>& getBodies() const { return bodies; }

			inline int getLeft() const		{ return aabb.getLeft(); }
			inline int getRight() const		{ return aabb.getRight(); }
			inline int getTop() const		{ return aabb.getTop(); }
			inline int getBottom() const	{ return aabb.getBottom(); }
	};
}

#endif
