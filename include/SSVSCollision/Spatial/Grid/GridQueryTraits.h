// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_GRIDQUERYTRAITS
#define SSVSC_GRIDQUERYTRAITS

#include <vector>
#include <SFML/System.hpp>

namespace ssvsc
{
	class AABB;
	class Body;
	class Grid;

	namespace GridQueryTypes
	{
		template<typename TDerived, typename... TArgs> struct Base
		{
			GridQuery<TDerived, TArgs...>& query;
			Base(GridQuery<TDerived, TArgs...>& mQuery) : query(mQuery) { }
		};

		namespace Bodies
		{
			struct All { static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, sf::Vector2i& mIndex, int mGroupId); };
			struct Grouped { static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, sf::Vector2i& mIndex, int mGroupId); };
		}

		namespace Orthogonal
		{
			struct Left : public Base<Left>
			{
				Left(GridQuery<Left>& mQuery);

				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
			struct Right : public Base<Right>
			{
				Right(GridQuery<Right>& mQuery);

				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
			struct Up : public Base<Up>
			{
				Up(GridQuery<Up>& mQuery);

				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
			struct Down : public Base<Down>
			{
				Down(GridQuery<Down>& mQuery);

				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
		}

		struct RayCast : public Base<RayCast, sf::Vector2f>
		{
			int cellSize;
			sf::Vector2i next{0, 0};
			sf::Vector2f direction, deltaDist, increment, max;

			RayCast(GridQuery<RayCast, sf::Vector2f>& mQuery, sf::Vector2f mDirection);

			bool isValid();
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool misses(const AABB& mShape);
			void setOut(const AABB& mShape);
		};
	}
}

#endif
