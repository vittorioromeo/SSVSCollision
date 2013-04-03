// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_GRIDQUERYTRAITS
#define SSVSC_GRIDQUERYTRAITS

#include <vector>
#include <SFML/System.hpp>

namespace ssvsc
{
	class Grid;
	class GridQuery;
	class AABB;

	namespace QueryTraits
	{
		namespace Bodies
		{
			struct All { static void getBodies(GridQuery& mQuery, const std::string& mGroup); };
			struct Grouped { static void getBodies(GridQuery& mQuery, const std::string& mGroup); };
		}

		namespace Orthogonal
		{
			struct Left
			{
				static bool isValid(const GridQuery& mQuery);
				static void step(GridQuery& mQuery);
				static bool getSorting(const GridQuery& mQuery, const Body* mA, const Body* mB);
				static bool misses(GridQuery& mQuery, const AABB& mShape);
				static void setOut(GridQuery& mQuery, const AABB& mShape);
			};
			struct Right
			{
				static bool isValid(const GridQuery& mQuery);
				static void step(GridQuery& mQuery);
				static bool getSorting(const GridQuery& mQuery, const Body* mA, const Body* mB);
				static bool misses(GridQuery& mQuery, const AABB& mShape);
				static void setOut(GridQuery& mQuery, const AABB& mShape);
			};
			struct Up
			{
				static bool isValid(const GridQuery& mQuery);
				static void step(GridQuery& mQuery);
				static bool getSorting(const GridQuery& mQuery, const Body* mA, const Body* mB);
				static bool misses(GridQuery& mQuery, const AABB& mShape);
				static void setOut(GridQuery& mQuery, const AABB& mShape);
			};
			struct Down
			{
				static bool isValid(const GridQuery& mQuery);
				static void step(GridQuery& mQuery);
				static bool getSorting(const GridQuery& mQuery, const Body* mA, const Body* mB);
				static bool misses(GridQuery& mQuery, const AABB& mShape);
				static void setOut(GridQuery& mQuery, const AABB& mShape);
			};
		}

		struct RayCast
		{
			static bool isValid(const GridQuery& mQuery);
			static void step(GridQuery& mQuery);
			static bool getSorting(const GridQuery& mQuery, const Body* mA, const Body* mB);
			static bool misses(GridQuery& mQuery, const AABB& mShape);
			static void setOut(GridQuery& mQuery, const AABB& mShape);
		};
	}
}

#endif
