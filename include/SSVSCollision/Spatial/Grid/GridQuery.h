// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

// TODO: create a resolver-independent querybase class

#ifndef SSVSC_SPATIAL_GRIDQUERY
#define SSVSC_SPATIAL_GRIDQUERY

#include <vector>
#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Spatial/Grid/Grid.h"
#include "SSVSCollision/Spatial/Grid/Cell.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Global/Typedefs.h"

namespace ssvsc
{
	namespace GridQueryTypes
	{
		namespace Bodies { struct All; struct Grouped; }
		namespace Orthogonal { struct Left; struct Right; struct Up; struct Down; }
		struct Point; struct RayCast; struct Distance;
		template<typename TDerived> struct Base;
	}

	template<typename T> class GridQuery
	{
		friend struct GridQueryTypes::Orthogonal::Left;
		friend struct GridQueryTypes::Orthogonal::Right;
		friend struct GridQueryTypes::Orthogonal::Up;
		friend struct GridQueryTypes::Orthogonal::Down;
		friend struct GridQueryTypes::RayCast;
		friend struct GridQueryTypes::Distance;
		friend struct GridQueryTypes::Point;
		template<typename TDerived> friend struct GridQueryTypes::Base;

		private:
			Grid& grid;
			std::vector<Body*> bodies;
			std::vector<Vec2i> visitedIndexes;
			T internal;

			template<typename TCellTraits> Body* nextImpl(int mGroupUid = -1)
			{
				while(internal.isValid())
				{
					if(bodies.empty())
					{
						TCellTraits::getBodies(bodies, grid, internal.index, mGroupUid);
						ssvu::sort(bodies, [&](const Body* mA, const Body* mB){ return internal.getSorting(mA, mB); });
						visitedIndexes.push_back(internal.index);
						internal.step();
					}

					while(!bodies.empty())
					{
						Body* body{bodies.back()};
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
			template<typename... TArgs> GridQuery(Grid& mGrid, TArgs... mArgs) : grid(mGrid), internal(*this, std::forward<TArgs>(mArgs)...) { }

			inline Body* next()				{ return nextImpl<GridQueryTypes::Bodies::All>(); }
			inline Body* next(Group mGroup)	{ return nextImpl<GridQueryTypes::Bodies::Grouped>(mGroup); }
			inline std::vector<Cell*> getAllCells()
			{
				std::vector<Cell*> result;

				while(internal.isValid())
				{
					result.push_back(&grid.getCell(internal.index));
					internal.step();
				}

				return result;
			}

			inline void reset()
			{
				bodies.clear();
				visitedIndexes.clear();
				internal.reset();
			}

			inline const Vec2f& getLastPos()						{ return internal.getLastPos(); }
			inline const std::vector<Vec2i>& getVisitedIndexes()	{ return visitedIndexes; }
	};
}

#endif
