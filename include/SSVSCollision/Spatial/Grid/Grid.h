// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID
#define SSVSC_SPATIAL_GRID

#include <vector>
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Query/Query.h"

namespace ssvsc
{
	template<typename> class Cell;

	namespace Internal
	{
		template<typename TContainer, typename TS> class GridBase
		{
			public:
				using TSInfo = GridInfo<TS>;

			protected:
				TContainer cells;
				int columns, rows, cellSize, offset;

			public:
				GridBase(int mColumns, int mRows, int mCellSize, int mOffset = 0) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset} { }

				inline int getIndexXMin() const noexcept	{ return 0 - offset; }
				inline int getIndexYMin() const noexcept	{ return 0 - offset; }
				inline int getIndexXMax() const noexcept	{ return columns - offset; }
				inline int getIndexYMax() const noexcept	{ return rows - offset; }
				inline int getRows() const noexcept			{ return rows; }
				inline int getColumns() const noexcept		{ return columns; }
				inline int getOffset() const noexcept		{ return offset; }
				inline int getCellSize() const noexcept		{ return cellSize; }

				inline int getIndex(int mValue) const noexcept					{ return mValue / cellSize; }
				inline Vec2i getIndex(const Vec2i& mPosition) const noexcept	{ return {getIndex(mPosition.x), getIndex(mPosition.y)}; }

				inline Cell<TS>& getCell(int mX, int mY)	{ return cells[ssvu::get1DIndexFrom2D(mX + offset, mY + offset, columns)]; }
				inline Cell<TS>& getCell(const Vec2i& mIdx)	{ return getCell(mIdx.x, mIdx.y); }

				inline const decltype(cells)& getCells() noexcept { return cells; }
				inline bool isIndexValid(const Vec2i& mIdx) const noexcept								{ return mIdx.x >= getIndexXMin() && mIdx.x < getIndexXMax() && mIdx.y >= getIndexYMin() && mIdx.y < getIndexYMax(); }
				inline bool isIndexValid(int mStartX, int mStartY, int mEndX, int mEndY) const noexcept	{ return mStartX >= getIndexXMin() && mEndX < getIndexXMax() && mStartY >= getIndexYMin() && mEndY < getIndexYMax(); }
		};
	}

	struct Grid : public Internal::GridBase<std::vector<Cell<Grid>>, Grid>
	{
		Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0) : Internal::GridBase<std::vector<Cell<Grid>>, Grid>{mColumns, mRows, mCellSize, mOffset} { cells.reserve(columns * rows); }
	};
	struct HashGrid : public Internal::GridBase<std::unordered_map<int, Cell<HashGrid>>, HashGrid>
	{
		HashGrid(int mColumns, int mRows, int mCellSize, int mOffset = 0) : Internal::GridBase<std::unordered_map<int, Cell<HashGrid>>, HashGrid>{mColumns, mRows, mCellSize, mOffset} { }
	};

	namespace GridQueryTypes
	{
		template<typename TS> struct Point;
		template<typename TS> struct Distance;
		template<typename TS> struct RayCast;
		template<typename TS> struct OrthoLeft;
		template<typename TS> struct OrthoRight;
		template<typename TS> struct OrthoUp;
		template<typename TS> struct OrthoDown;
		namespace Bodies { struct All; struct ByGroup; }
	}

	template<typename TS> struct QueryTypeDispatcher<TS, QueryType::Point>		{ using Type = GridQueryTypes::Point<TS>; };
	template<typename TS> struct QueryTypeDispatcher<TS, QueryType::Distance>	{ using Type = GridQueryTypes::Distance<TS>; };
	template<typename TS> struct QueryTypeDispatcher<TS, QueryType::RayCast>	{ using Type = GridQueryTypes::RayCast<TS>; };
	template<typename TS> struct QueryTypeDispatcher<TS, QueryType::OrthoLeft>	{ using Type = GridQueryTypes::OrthoLeft<TS>; };
	template<typename TS> struct QueryTypeDispatcher<TS, QueryType::OrthoRight>	{ using Type = GridQueryTypes::OrthoRight<TS>; };
	template<typename TS> struct QueryTypeDispatcher<TS, QueryType::OrthoUp>	{ using Type = GridQueryTypes::OrthoUp<TS>; };
	template<typename TS> struct QueryTypeDispatcher<TS, QueryType::OrthoDown>	{ using Type = GridQueryTypes::OrthoDown<TS>; };

	template<typename TS> struct QueryModeDispatcher<TS, QueryMode::All>		{ using Type = GridQueryTypes::Bodies::All; };
	template<typename TS> struct QueryModeDispatcher<TS, QueryMode::ByGroup>	{ using Type = GridQueryTypes::Bodies::ByGroup; };
}

#endif
