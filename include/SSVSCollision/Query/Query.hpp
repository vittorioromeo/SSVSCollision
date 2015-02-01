// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_QUERY
#define SSVSC_QUERY

namespace ssvsc
{
	template<typename TW> class Body;

	template<typename TW, typename TS, QueryType TType> struct QueryTypeDispatcher;
	template<typename TW, typename TS, QueryMode TMode> struct QueryModeDispatcher;

	template<typename TW, typename TInternal, typename TMode> class Query
	{
		public:
			using BodyType = Body<TW>;
			friend TInternal;

		private:
			std::vector<BodyType*> bodies;
			TInternal internal;

		public:
			template<typename... TArgs> inline Query(TArgs&&... mArgs) noexcept : internal{SSVU_FWD(mArgs)...} { }
			template<typename... TArgs> BodyType* next(TArgs&&... mArgs)
			{
				while(internal.isValid())
				{
					// If the body stack is empty, 'refill' it using TMode::getBodies, then sort
					if(bodies.empty())
					{
						TMode::getBodies(bodies, internal, SSVU_FWD(mArgs)...);
						ssvu::sort(bodies, [this](const BodyType* mA, const BodyType* mB){ return internal.getSorting(mA, mB); });
						internal.step();
					}

					// While the body stack is not empty, 'yield' bodies one by one
					while(!bodies.empty())
					{
						BodyType* body{bodies.back()};
						const auto& shape(body->getShape());
						bodies.pop_back();

						if(!internal.hits(shape)) continue;

						internal.setOut(shape);
						return body;
					}
				}

				return nullptr;
			}

			inline void reset() { bodies.clear(); internal.reset(); }
			inline const auto& getLastPos() const noexcept { return internal.getLastPos(); }
	};
}

#endif
