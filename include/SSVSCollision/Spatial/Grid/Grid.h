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

	class Grid : public SpatialBase
	{
		private:
			ssvu::MemoryManager<GridInfo> gridInfos;
			std::vector<Uptr<Cell>> cells;
			int columns, rows, cellSize, offset;

		public:
			Grid(int mColumns, int mRows, int mCellSize, int mOffset = 0) : columns{mColumns}, rows{mRows}, cellSize{mCellSize}, offset{mOffset}
			{
				for(int iX{0}; iX < columns; ++iX)
					for(int iY{0}; iY < rows; ++iY)
					{
						int left{iX * cellSize}, right{cellSize + left}, top{iY * cellSize}, bottom{cellSize + top};
						cells.emplace_back(new Cell{{left, right, top, bottom}});
					}
			}

			inline SpatialInfoBase& createSpatialInfo(Base& mBase) override { return gridInfos.create(*this, mBase); }
			inline void refresh() override { gridInfos.refresh(); }
			inline void del(SpatialInfoBase& mSpatialInfo) override { gridInfos.del(static_cast<GridInfo&>(mSpatialInfo)); }

			inline int getIndexXMin() const	{ return 0 - offset; }
			inline int getIndexYMin() const	{ return 0 - offset; }
			inline int getIndexXMax() const	{ return columns - offset; }
			inline int getIndexYMax() const	{ return rows - offset; }
			inline int getRows() const		{ return rows; }
			inline int getColumns() const	{ return columns; }
			inline int getOffset() const	{ return offset; }
			inline int getCellSize() const	{ return cellSize; }

			inline int getIndex(int mValue) const			{ return mValue / cellSize; }
			inline Vec2i getIndex(Vec2i mPosition) const	{ return {getIndex(mPosition.x), getIndex(mPosition.y)}; }

			inline Cell& getCell(int mX, int mY)	{ return *cells[ssvu::get1DIndexFrom2D(mX + offset, mY + offset, columns)]; }
			inline Cell& getCell(Vec2i mIndex)		{ return getCell(mIndex.x, mIndex.y); }

			inline const decltype(cells)& getCells() { return cells; }
			inline bool isIndexValid(Vec2i mIndex) const									{ return mIndex.x >= getIndexXMin() && mIndex.x < getIndexXMax() && mIndex.y >= getIndexYMin() && mIndex.y < getIndexYMax(); }
			inline bool isIndexValid(int mStartX, int mStartY, int mEndX, int mEndY) const	{ return mStartX >= getIndexXMin() && mEndX < getIndexXMax() && mStartY >= getIndexYMin() && mEndY < getIndexYMax(); }
	};

	namespace GridQueryTypes
	{
		struct Point; struct Distance; struct RayCast;
		struct OrthoLeft; struct OrthoRight; struct OrthoUp; struct OrthoDown;
		namespace Bodies { struct All; struct ByGroup; }
	}

	template<> struct QueryTypeDispatcher<Grid, QueryType::Point>		{ using Type = GridQueryTypes::Point; };
	template<> struct QueryTypeDispatcher<Grid, QueryType::Distance>	{ using Type = GridQueryTypes::Distance; };
	template<> struct QueryTypeDispatcher<Grid, QueryType::RayCast>		{ using Type = GridQueryTypes::RayCast; };
	template<> struct QueryTypeDispatcher<Grid, QueryType::OrthoLeft>	{ using Type = GridQueryTypes::OrthoLeft; };
	template<> struct QueryTypeDispatcher<Grid, QueryType::OrthoRight>	{ using Type = GridQueryTypes::OrthoRight; };
	template<> struct QueryTypeDispatcher<Grid, QueryType::OrthoUp>		{ using Type = GridQueryTypes::OrthoUp; };
	template<> struct QueryTypeDispatcher<Grid, QueryType::OrthoDown>	{ using Type = GridQueryTypes::OrthoDown; };

	template<> struct QueryModeDispatcher<Grid, QueryMode::All>			{ using Type = GridQueryTypes::Bodies::All; };
	template<> struct QueryModeDispatcher<Grid, QueryMode::ByGroup>		{ using Type = GridQueryTypes::Bodies::ByGroup; };
}

#endif
