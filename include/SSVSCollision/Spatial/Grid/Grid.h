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
	template<typename TW> class Cell;

	namespace Internal
	{
		template<typename TW, typename TContainer, typename TDerived> class GridBase
		{
			public:
				using CellType = Cell<TW>;
				using SpatialInfoType = GridInfo<TW>;

			protected:
				TContainer cells;
				int columns, rows, cellSize, offset;

			public:
				inline GridBase(int mColumns, int mRows, int mCellSize, int mOffset = 0) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset} { }

				inline int getIndexXMin() const noexcept	{ return 0 - offset; }
				inline int getIndexYMin() const noexcept	{ return 0 - offset; }
				inline int getIndexXMax() const noexcept	{ return columns - offset; }
				inline int getIndexYMax() const noexcept	{ return rows - offset; }
				inline int getRows() const noexcept			{ return rows; }
				inline int getColumns() const noexcept		{ return columns; }
				inline int getOffset() const noexcept		{ return offset; }
				inline int getCellSize() const noexcept		{ return cellSize; }

				inline int getIndex(int mValue) const noexcept			{ return mValue / cellSize; }
				inline Vec2i getIndex(const Vec2i& mPos) const noexcept	{ return {getIndex(mPos.x), getIndex(mPos.y)}; }

				inline const CellType& getCell(int mX, int mY) const	{ return cells.at(ssvu::get1DIndexFrom2D(mX + offset, mY + offset, columns)); }
				inline CellType& getCell(int mX, int mY)				{ return cells[ssvu::get1DIndexFrom2D(mX + offset, mY + offset, columns)]; }
				inline const CellType& getCell(const Vec2i& mIdx) const	{ return getCell(mIdx.x, mIdx.y); }
				inline CellType& getCell(const Vec2i& mIdx)				{ return getCell(mIdx.x, mIdx.y); }

				inline const decltype(cells)& getCells() const noexcept { return cells; }
				inline decltype(cells)& getCells() noexcept				{ return cells; }

				inline bool isIndexValid(const Vec2i& mIdx) const noexcept					{ return mIdx.x >= getIndexXMin() && mIdx.x < getIndexXMax() && mIdx.y >= getIndexYMin() && mIdx.y < getIndexYMax(); }
				inline bool isIndexValid(int mX1, int mY1, int mX2, int mY2) const noexcept	{ return mX1 >= getIndexXMin() && mX2 < getIndexXMax() && mY1 >= getIndexYMin() && mY2 < getIndexYMax(); }
		};
	}

	template<typename TW> struct Grid : public Internal::GridBase<TW, std::vector<Cell<TW>>, Grid<TW>>
	{
		Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0) : Internal::GridBase<TW, std::vector<Cell<TW>>, Grid<TW>>{mColumns, mRows, mCellSize, mOffset} { this->cells.reserve(this->columns * this->rows); }
	};
	template<typename TW> struct HashGrid : public Internal::GridBase<TW, std::unordered_map<int, Cell<TW>>, HashGrid<TW>>
	{
		HashGrid(int mColumns, int mRows, int mCellSize, int mOffset = 0) : Internal::GridBase<TW, std::unordered_map<int, Cell<TW>>, HashGrid<TW>>{mColumns, mRows, mCellSize, mOffset} { }
	};

	namespace GridQueryTypes
	{
		template<typename TW, typename TGrid> struct Point;
		template<typename TW, typename TGrid> struct Distance;
		template<typename TW, typename TGrid> struct RayCast;
		template<typename TW, typename TGrid> struct OrthoLeft;
		template<typename TW, typename TGrid> struct OrthoRight;
		template<typename TW, typename TGrid> struct OrthoUp;
		template<typename TW, typename TGrid> struct OrthoDown;
		namespace Bodies { template<typename TW> struct All; template<typename TW> struct ByGroup; }
	}

	template<typename TW, typename TGrid> struct QueryTypeDispatcher<TW, TGrid, QueryType::Point>		{ using Type = GridQueryTypes::Point<TW, TGrid>; };
	template<typename TW, typename TGrid> struct QueryTypeDispatcher<TW, TGrid, QueryType::Distance>	{ using Type = GridQueryTypes::Distance<TW, TGrid>; };
	template<typename TW, typename TGrid> struct QueryTypeDispatcher<TW, TGrid, QueryType::RayCast>		{ using Type = GridQueryTypes::RayCast<TW, TGrid>; };
	template<typename TW, typename TGrid> struct QueryTypeDispatcher<TW, TGrid, QueryType::OrthoLeft>	{ using Type = GridQueryTypes::OrthoLeft<TW, TGrid>; };
	template<typename TW, typename TGrid> struct QueryTypeDispatcher<TW, TGrid, QueryType::OrthoRight>	{ using Type = GridQueryTypes::OrthoRight<TW, TGrid>; };
	template<typename TW, typename TGrid> struct QueryTypeDispatcher<TW, TGrid, QueryType::OrthoUp>		{ using Type = GridQueryTypes::OrthoUp<TW, TGrid>; };
	template<typename TW, typename TGrid> struct QueryTypeDispatcher<TW, TGrid, QueryType::OrthoDown>	{ using Type = GridQueryTypes::OrthoDown<TW, TGrid>; };

	template<typename TW, typename TGrid> struct QueryModeDispatcher<TW, TGrid, QueryMode::All>			{ using Type = GridQueryTypes::Bodies::All<TW>; };
	template<typename TW, typename TGrid> struct QueryModeDispatcher<TW, TGrid, QueryMode::ByGroup>		{ using Type = GridQueryTypes::Bodies::ByGroup<TW>; };
}

#endif
