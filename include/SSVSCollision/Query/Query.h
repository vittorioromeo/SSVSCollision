// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

// TODO: create a resolver-independent querybase class

#ifndef SSVSC_QUERY
#define SSVSC_QUERY

#include <vector>
#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	class Body;

	enum class QueryType{Point, Distance, RayCast, OrthoLeft, OrthoRight, OrthoUp, OrthoDown};
	enum class QueryMode{All};

	template<typename TSpatial, QueryType TType> struct QueryTypeDispatcher;
	template<typename TSpatial, QueryMode TMode> struct QueryModeDispatcher;

	template<typename TSpatial, typename TType, typename TMode> class Query
	{
		friend TSpatial;
		friend TType;

		private:
			std::vector<Body*> bodies;
			TType internal;

			template<typename TBody = Body> Body* nextImpl(int mGroupUid = -1)
			{
				while(internal.isValid())
				{
					if(bodies.empty())
					{
						TMode::getBodies(bodies, internal.grid, internal.index, mGroupUid);
						ssvu::sort(bodies, [&](const Body* mA, const Body* mB){ return internal.getSorting(mA, mB); });
						internal.step();
					}

					while(!bodies.empty())
					{
						TBody* body{bodies.back()};
						const auto& shape(body->getShape());
						bodies.pop_back();

						if(!internal.hits(shape)) continue;

						internal.setOut(shape);
						return body;
					}
				}

				return nullptr;
			}

		public:
			template<typename... TArgs> Query(TArgs&&... mArgs) : internal(std::forward<TArgs>(mArgs)...) { }

			inline Body* next()				{ return nextImpl(); }

			inline void reset() { bodies.clear(); internal.reset(); }
			inline const Vec2f& getLastPos() { return internal.getLastPos(); }
	};
}

#endif
