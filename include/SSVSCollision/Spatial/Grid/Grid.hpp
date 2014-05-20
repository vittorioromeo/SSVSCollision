// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID
#define SSVSC_SPATIAL_GRID

#include "SSVSCollision/Spatial/Grid/Cell.hpp"
#include "SSVSCollision/Spatial/Grid/GridInfo.hpp"

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
				int cols, rows, cellSize, offset;

			public:
				inline GridBase(int mCols, int mRows, int mCellSize, int mOffset = 0) : cols{mCols}, rows{mRows}, cellSize{mCellSize}, offset{mOffset} { }

				inline int getIdxXMin() const noexcept	{ return 0 - offset; }
				inline int getIdxYMin() const noexcept	{ return 0 - offset; }
				inline int getIdxXMax() const noexcept	{ return cols - offset; }
				inline int getIdxYMax() const noexcept	{ return rows - offset; }
				inline int getRows() const noexcept		{ return rows; }
				inline int getColumns() const noexcept	{ return cols; }
				inline int getOffset() const noexcept	{ return offset; }
				inline int getCellSize() const noexcept	{ return cellSize; }

				inline int getIdx(int mValue) const noexcept			{ SSVU_ASSERT(cellSize != 0); return mValue / cellSize; }
				inline Vec2i getIdx(const Vec2i& mPos) const noexcept	{ return {getIdx(mPos.x), getIdx(mPos.y)}; }

				inline const CellType& getCell(int mX, int mY) const	{ return cells.at(ssvu::get1DIdxFrom2D(mX + offset, mY + offset, cols)); }
				inline CellType& getCell(int mX, int mY)				{ return cells[ssvu::get1DIdxFrom2D(mX + offset, mY + offset, cols)]; }
				inline const CellType& getCell(const Vec2i& mIdx) const	{ return getCell(mIdx.x, mIdx.y); }
				inline CellType& getCell(const Vec2i& mIdx)				{ return getCell(mIdx.x, mIdx.y); }

				inline const decltype(cells)& getCells() const noexcept { return cells; }
				inline decltype(cells)& getCells() noexcept				{ return cells; }

				inline bool isIdxValid(const Vec2i& mIdx) const noexcept					{ return mIdx.x >= getIdxXMin() && mIdx.x < getIdxXMax() && mIdx.y >= getIdxYMin() && mIdx.y < getIdxYMax(); }
				inline bool isIdxValid(int mX1, int mY1, int mX2, int mY2) const noexcept	{ return mX1 >= getIdxXMin() && mX2 < getIdxXMax() && mY1 >= getIdxYMin() && mY2 < getIdxYMax(); }
		};

		struct HashGridHash
		{
			inline std::size_t operator()(int mKey) const noexcept { return mKey; }
		};
		struct HashGridEqual
		{
			inline bool operator()(int mA, int mB) const noexcept { return mA == mB; }
		};

		template<typename TW> using HashGridType = std::unordered_map<int, Cell<TW>, HashGridHash, HashGridEqual>;
		template<typename TW> using GridType = std::vector<Cell<TW>>;
	}

	template<typename TW> struct Grid final : public Internal::GridBase<TW, Internal::GridType<TW>, Grid<TW>>
	{
		inline Grid(int mCols, int mRows, int mCellSize, int mOffset = 0)
			: Internal::GridBase<TW, Internal::GridType<TW>, Grid<TW>>{mCols, mRows, mCellSize, mOffset}
		{
			this->cells.reserve(this->cols * this->rows);
		}
	};

	template<typename TW> struct HashGrid final : public Internal::GridBase<TW, Internal::HashGridType<TW>, HashGrid<TW>>
	{
		inline HashGrid(int mCols, int mRows, int mCellSize, int mOffset = 0)
			: Internal::GridBase<TW, Internal::HashGridType<TW>, HashGrid<TW>>{mCols, mRows, mCellSize, mOffset}
		{

		}
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

#include "SSVSCollision/Spatial/Grid/GridQueryTypes.hpp"

#endif
