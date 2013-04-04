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
	template<typename T, typename... TArgs> struct GridCRTPQuery
	{
		Grid& grid;
		sf::Vector2f startPos, pos, out;
		sf::Vector2i startIndex, index;
		std::vector<Body*> bodies;
		std::vector<sf::Vector2i> visitedIndexes;
		T traitBase;

		GridCRTPQuery(Grid& mGrid, sf::Vector2i mStartPos, TArgs... mArgs) : grid(mGrid), startPos{sf::Vector2f(mStartPos)}, pos{startPos},
			startIndex{grid.getIndex(mStartPos)}, index{startIndex}, traitBase(*this, mArgs...) { }

		template<typename TCellTraits> Body* nextImpl(const std::string& mGroup = "")
		{
			while(traitBase.isValid())
			{
				if(bodies.empty())
				{
					visitedIndexes.push_back(index);
					TCellTraits::getBodies(bodies, grid, index, mGroup);
					traitBase.step();
					ssvu::sort(bodies, [&](const Body* mA, const Body* mB){ return traitBase.getSorting(mA, mB); });
				}

				while(!bodies.empty())
				{
					Body* body{bodies.back()};
					auto& shape(body->getShape());
					bodies.pop_back();

					if(traitBase.misses(shape)) continue;

					traitBase.setOut(shape);
					return body;
				}
			}

			return nullptr;
		}

		Body* next() { return nextImpl<QueryTraits::Bodies::All>(); }
		Body* next(const std::string& mGroup) { return nextImpl<QueryTraits::Bodies::Grouped>(mGroup); }

		void reset()
		{
			pos = startPos;
			index = startIndex;
			bodies.clear();
			visitedIndexes.clear();
		}

		// Getters
		const sf::Vector2f& getOut() { return out; }
		const std::vector<sf::Vector2i>& getVisitedIndexes() { return visitedIndexes; }
	};
}

#endif
