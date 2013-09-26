// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_QUERY
#define SSVSC_QUERY

#include <vector>
#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	template<typename> class BodyType;

	template<typename TS, QueryType TType> struct QueryTypeDispatcher;
	template<typename TS, QueryMode TMode> struct QueryModeDispatcher;

	template<typename TS, typename TType, typename TMode> class Query
	{
		friend TS;
		friend TType;

		public:
			using Body = BodyType<TS>;

		private:
			std::vector<Body*> bodies;
			TType internal;

			template<typename TBody = Body, typename... TArgs> TBody* nextImpl(TArgs&&... mArgs)
			{
				while(internal.isValid())
				{
					// If the body stack is empty, 'refill' it using TMode::getBodies, then sort
					if(bodies.empty())
					{
						TMode::getBodies(bodies, internal, std::forward<TArgs>(mArgs)...);
						ssvu::sort(bodies, [this](const TBody* mA, const TBody* mB){ return internal.getSorting(mA, mB); });
						internal.step();
					}

					// While the body stack is not empty, 'yield' bodies one by one
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
			template<typename... TArgs> inline Body* next(TArgs&&... mArgs) { return nextImpl(std::forward<TArgs>(mArgs)...); }

			inline void reset() { bodies.clear(); internal.reset(); }
			inline const Vec2f& getLastPos() { return internal.getLastPos(); }
	};
}

#endif
