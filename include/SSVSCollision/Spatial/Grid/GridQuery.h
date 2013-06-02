// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_GRIDQUERY
#define SSVSC_GRIDQUERY

#include <vector>
#include <SFML/System.hpp>
#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/Grid/GridQueryTraits.h"

namespace ssvsc
{
	template<typename T, typename... TArgs> class GridQuery
	{
		friend struct GridQueryTypes::Orthogonal::Left;
		friend struct GridQueryTypes::Orthogonal::Right;
		friend struct GridQueryTypes::Orthogonal::Up;
		friend struct GridQueryTypes::Orthogonal::Down;
		friend struct GridQueryTypes::RayCast;
		friend struct GridQueryTypes::Distance;

		private:
			Grid& grid;
			sf::Vector2f startPos, pos, lastPos;
			sf::Vector2i startIndex, index;
			std::vector<Body*> bodies;
			std::vector<sf::Vector2i> visitedIndexes;
			T internal;

			template<typename TCellTraits> Body* nextImpl(int mGroupUid = -1)
			{
				while(internal.isValid())
				{
					if(bodies.empty())
					{
						TCellTraits::getBodies(bodies, grid, index, mGroupUid);
						ssvu::sort(bodies, [&](const Body* mA, const Body* mB){ return internal.getSorting(mA, mB); });
						visitedIndexes.push_back(index);
						internal.step();
					}

					while(!bodies.empty())
					{
						Body* body{bodies.back()};
						const auto& shape(body->getShape());
						bodies.pop_back();

						if(internal.misses(shape)) continue;

						internal.setOut(shape);
						return body;
					}
				}

				return nullptr;
			}



		public:
			GridQuery(Grid& mGrid, sf::Vector2i mStartPos, TArgs... mArgs) : grid(mGrid), startPos{sf::Vector2f(mStartPos)}, pos{startPos},
				startIndex{grid.getIndex(mStartPos)}, index{startIndex}, internal(*this, mArgs...) { }

			Body* next() { return nextImpl<GridQueryTypes::Bodies::All>(); }
			Body* next(int mGroupId) { return nextImpl<GridQueryTypes::Bodies::Grouped>(mGroupId); }
			std::vector<Cell*> getAllCells()
			{
				std::vector<Cell*> result;

				while(internal.isValid())
				{
					result.push_back(&grid.getCell(index));
					internal.step();
				}

				return result;
			}

			void reset()
			{
				pos = startPos;
				index = startIndex;
				bodies.clear();
				visitedIndexes.clear();
				// TODO: call internal.reset() ?
			}

			// Getters
			const sf::Vector2f& getLastPos() { return lastPos; }
			const std::vector<sf::Vector2i>& getVisitedIndexes() { return visitedIndexes; }
	};
}

#endif
