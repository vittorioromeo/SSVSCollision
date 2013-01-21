#ifndef GRID_H
#define GRID_H

#include <vector>

namespace ssvsc
{
	class Cell;

	class Grid
	{
		private:
			std::vector<std::vector<Cell*>> cells; // owned
			int columns, rows, cellSize, offset;

		public:
			Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0);
			~Grid();

			void update(float mFrameTime);

			Cell* getCell(int mX, int mY);
			int getIndex(int mValue);
			bool isOutside(int mStartX, int mStartY, int mEndX, int mEndY);
	};
}

#endif // GRID_H
