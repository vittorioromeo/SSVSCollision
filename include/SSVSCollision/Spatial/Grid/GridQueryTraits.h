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
	template<typename T, typename... TArgs> class GridCRTPQuery;
	class AABB;
	class Body;

	namespace QueryTraits
	{
		template<typename TDerived, typename... TArgs> struct QueryTraitBase
		{
			GridCRTPQuery<TDerived, TArgs...>& query;
			
			QueryTraitBase(GridCRTPQuery<TDerived, TArgs...>& mQuery) : query(mQuery) { }

		};
		
		namespace Bodies
		{
			struct All { static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, sf::Vector2i& mIndex, const std::string& mGroup); };
			struct Grouped { static void getBodies(std::vector<Body*>& mBodies, Grid& mGrid, sf::Vector2i& mIndex, const std::string& mGroup); };
		}

		namespace Orthogonal
		{
			struct Left : public QueryTraitBase<Left>					
			{								
				Left(GridCRTPQuery<Left>& mQuery);
					
				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
			struct Right : public QueryTraitBase<Right>
			{	
				Right(GridCRTPQuery<Right>& mQuery);					
					
				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
			struct Up : public QueryTraitBase<Up>
			{		
				Up(GridCRTPQuery<Up>& mQuery);
					
				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
			struct Down : public QueryTraitBase<Down>
			{										
				Down(GridCRTPQuery<Down>& mQuery);
					
				bool isValid();
				void step();
				bool getSorting(const Body* mA, const Body* mB);
				bool misses(const AABB& mShape);
				void setOut(const AABB& mShape);
			};
		}

		struct RayCast : public QueryTraitBase<RayCast, sf::Vector2f>
		{
			int cellSize;
			sf::Vector2i stepVec;
			sf::Vector2f direction, deltaDist, max, increment;
						
			RayCast(GridCRTPQuery<RayCast, sf::Vector2f>& mQuery, sf::Vector2f mDirection);

			bool isValid();
			void step();
			bool getSorting(const Body* mA, const Body* mB);
			bool misses(const AABB& mShape);
			void setOut(const AABB& mShape);
		};
	}
}

#endif
