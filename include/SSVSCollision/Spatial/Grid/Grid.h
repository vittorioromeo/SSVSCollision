// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID
#define SSVSC_SPATIAL_GRID

#include <vector>
#include "SSVSCollision/Spatial/Grid/GridInfo.h"
#include "SSVSCollision/Spatial/SpatialBase.h"
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Query/Query.h"

namespace ssvsc
{
	class Cell;
	class SpatialInfoBase;

	namespace Internal
	{
		template<typename TContainer, typename TDerived> class GridBase : public SpatialBase
		{
			protected:
				ssvu::MemoryManager<GridInfo<TDerived>> gridInfos;
				TContainer cells;
				int columns, rows, cellSize, offset;

			public:
				GridBase(int mColumns, int mRows, int mCellSize, int mOffset = 0) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset} { }

				inline SpatialInfoBase& createSpatialInfo(Base& mBase) override	{ return gridInfos.create(static_cast<TDerived&>(*this), mBase); }
				inline void refresh() override									{ gridInfos.refresh(); }
				inline void del(SpatialInfoBase& mSpatialInfo) override			{ gridInfos.del(static_cast<GridInfo<TDerived>&>(mSpatialInfo)); }

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

				inline Cell& getCell(int mX, int mY)	{ return cells[ssvu::get1DIndexFrom2D(mX + offset, mY + offset, columns)]; }
				inline Cell& getCell(const Vec2i& mIdx)	{ return getCell(mIdx.x, mIdx.y); }

				inline const decltype(cells)& getCells() const noexcept { return cells; }
				inline decltype(cells)& getCells() noexcept				{ return cells; }

				inline bool isIndexValid(const Vec2i& mIdx) const noexcept					{ return mIdx.x >= getIndexXMin() && mIdx.x < getIndexXMax() && mIdx.y >= getIndexYMin() && mIdx.y < getIndexYMax(); }
				inline bool isIndexValid(int mX1, int mY1, int mX2, int mY2) const noexcept	{ return mX1 >= getIndexXMin() && mX2 < getIndexXMax() && mY1 >= getIndexYMin() && mY2 < getIndexYMax(); }
		};
	}

	struct Grid : public Internal::GridBase<std::vector<Cell>, Grid>
	{
		Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0) : Internal::GridBase<std::vector<Cell>, Grid>{mColumns, mRows, mCellSize, mOffset} { cells.reserve(columns * rows); }
	};
	struct HashGrid : public Internal::GridBase<std::unordered_map<int, Cell>, HashGrid>
	{
		HashGrid(int mColumns, int mRows, int mCellSize, int mOffset = 0) : Internal::GridBase<std::unordered_map<int, Cell>, HashGrid>{mColumns, mRows, mCellSize, mOffset} { }
	};

	namespace GridQueryTypes
	{
		template<typename TGrid> struct Point;
		template<typename TGrid> struct Distance;
		template<typename TGrid> struct RayCast;
		template<typename TGrid> struct OrthoLeft;
		template<typename TGrid> struct OrthoRight;
		template<typename TGrid> struct OrthoUp;
		template<typename TGrid> struct OrthoDown;
		namespace Bodies { struct All; struct ByGroup; }
	}

	template<typename TGrid> struct QueryTypeDispatcher<TGrid, QueryType::Point>		{ using Type = GridQueryTypes::Point<TGrid>; };
	template<typename TGrid> struct QueryTypeDispatcher<TGrid, QueryType::Distance>		{ using Type = GridQueryTypes::Distance<TGrid>; };
	template<typename TGrid> struct QueryTypeDispatcher<TGrid, QueryType::RayCast>		{ using Type = GridQueryTypes::RayCast<TGrid>; };
	template<typename TGrid> struct QueryTypeDispatcher<TGrid, QueryType::OrthoLeft>	{ using Type = GridQueryTypes::OrthoLeft<TGrid>; };
	template<typename TGrid> struct QueryTypeDispatcher<TGrid, QueryType::OrthoRight>	{ using Type = GridQueryTypes::OrthoRight<TGrid>; };
	template<typename TGrid> struct QueryTypeDispatcher<TGrid, QueryType::OrthoUp>		{ using Type = GridQueryTypes::OrthoUp<TGrid>; };
	template<typename TGrid> struct QueryTypeDispatcher<TGrid, QueryType::OrthoDown>	{ using Type = GridQueryTypes::OrthoDown<TGrid>; };

	template<typename TGrid> struct QueryModeDispatcher<TGrid, QueryMode::All>			{ using Type = GridQueryTypes::Bodies::All; };
	template<typename TGrid> struct QueryModeDispatcher<TGrid, QueryMode::ByGroup>		{ using Type = GridQueryTypes::Bodies::ByGroup; };
}

#endif
