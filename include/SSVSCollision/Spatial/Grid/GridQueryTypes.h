// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRIDQUERYTYPES
#define SSVSC_SPATIAL_GRIDQUERYTYPES

#include <vector>
#include <queue>
#include "SSVSCollision/Global/Typedefs.h"

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
			struct All { static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, Vec2i& mIndex, int mGroupUid); };
			struct Grouped { static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, Vec2i& mIndex, int mGroupUid); };
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

		struct RayCast : public Base<RayCast, Vec2f>
		{
			int cellSize;
			Vec2i next{0, 0};
			Vec2f direction, deltaDist, increment, max;

			RayCast(GridQuery<RayCast, Vec2f>& mQuery, Vec2f mDirection);

			bool isValid();
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool misses(const AABB& mShape);
			void setOut(const AABB& mShape);
		};

		struct Distance : public Base<Distance, int>
		{
			int cellSize, distance, cellRadius;
			std::queue<Vec2i> offsets;

			Distance(GridQuery<Distance, int>& mQuery, int mDistance);

			bool isValid();
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool misses(const AABB& mShape);
			void setOut(const AABB& mShape);
		};
	}
}

#endif
