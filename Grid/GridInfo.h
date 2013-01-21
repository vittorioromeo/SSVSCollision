#ifndef GRIDINFO_H
#define GRIDINFO_H

#include <vector>
#include <sparsehash/dense_hash_set>

namespace ssvsc
{
	class Body;
	class Grid;
	class Cell;

	class GridInfo
	{
		private:
			Grid& grid;
			Body& body;

			std::vector<Cell*> cells;
			std::vector<std::vector<Body*>*> queries; // Cell vector ptrs to query
			int startX, startY, endX, endY; // Edge cell positions
			bool mustRecalculate{false}; // IF TRUE CRASHES ON START - MUST FIX

			void recalcEdges(); // Sets startX, startY, endX, endY
			void checkOldEdges(); // Checks if startXY... is different from previousStartXY... - if so, recalculates
			void recalcCells(); // Clears cells/queries and gets new ones
			void clearCells();

		public:
			GridInfo(Grid& mGrid, Body& mBody);
			~GridInfo();

			void groupsChanged();
			void beforeUpdate();
			void afterUpdate();
			google::dense_hash_set<Body*> getBodiesToCheck();
	};
}

#endif // GRIDINFO_H
