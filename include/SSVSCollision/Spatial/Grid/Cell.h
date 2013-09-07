// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID_CELL
#define SSVSC_SPATIAL_GRID_CELL

#include <vector>
#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Body/Body.h"

namespace ssvsc
{
	class Cell
	{
		private:
			std::vector<Body*> bodies;

			inline void addBody(Body* mBody) { bodies.push_back(mBody); }
			inline void delBody(Body* mBody) { ssvu::eraseRemove(bodies, mBody); }

		public:
			inline void add(Base* mBase) { if(mBase->getType() == BaseType::Body) addBody(static_cast<Body*>(mBase)); }
			inline void del(Base* mBase) { if(mBase->getType() == BaseType::Body) delBody(static_cast<Body*>(mBase)); }

			inline const decltype(bodies)& getBodies() const noexcept { return bodies; }
	};
}

#endif
