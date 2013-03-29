// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_GRIDINFO
#define SSVSC_GRIDINFO

#include <vector>
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#pragma GCC system_header
#include <google/dense_hash_set>

namespace ssvsc
{
	class Body;
	class Grid;
	class Cell;

	class GridInfo : public SpatialInfoBase
	{
		private:
			Grid& grid;

			std::vector<Cell*> cells;
			std::vector<std::vector<Body*>*> queries; // Cell vector ptrs to query
			int startX{0}, startY{0}, endX{0}, endY{0}; // Edge cell positions
			int oldStartX{-1}, oldStartY{-1}, oldEndX{-1}, oldEndY{-1};
			bool invalid{true};

			void calcEdges(); // Sets startX, startY, endX, endY
			void checkEdges(); // Checks if startXY... is different from previousStartXY... - if so, recalculates
			void calcCells(); // Clears cells/queries and gets new ones
			void clear();

		public:
			GridInfo(Grid& mGrid, Body& mBody);
			~GridInfo();

			void invalidate() override;
			void preUpdate() override;
			void postUpdate() override;
			google::dense_hash_set<Body*> getBodiesToCheck() override;
			void destroy() override;
	};
}

#endif
