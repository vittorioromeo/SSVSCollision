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
	template<typename TW> class Cell
	{
		public:
			using BaseType = Base<TW>;
			using BodyType = Body<TW>;

		private:
			std::vector<BodyType*> bodies;

			inline void addBody(BodyType* mBody) { bodies.push_back(mBody); }
			inline void delBody(BodyType* mBody) { ssvu::eraseRemove(bodies, mBody); }

		public:
			inline void add(BaseType* mBase) { if(mBase->getType() == BBType::Body) addBody(static_cast<Body<TW>*>(mBase)); }
			inline void del(BaseType* mBase) { if(mBase->getType() == BBType::Body) delBody(static_cast<Body<TW>*>(mBase)); }

			inline const decltype(bodies)& getBodies() const noexcept { return bodies; }
	};
}

#endif
