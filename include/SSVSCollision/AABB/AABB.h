// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_AABB
#define SSVSC_AABB

#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Utils/Segment.h"
#include "SSVSCollision/Utils/Utils.h"

namespace ssvsc
{
	class AABB
	{
		private:
			Vec2i position, halfSize;

		public:
			inline AABB(const Vec2i& mPosition, const Vec2i& mHalfSize) noexcept : position{mPosition}, halfSize{mHalfSize} { }
			inline AABB(int mLeft, int mRight, int mTop, int mBottom) noexcept : position{mLeft + (mRight - mLeft) / 2, mTop + (mBottom - mTop) / 2}, halfSize{(mRight - mLeft) / 2, (mBottom - mTop) / 2} { }

			inline bool operator==(const AABB& mOther) const noexcept { return position == mOther.position && halfSize == mOther.halfSize; }
			inline bool operator!=(const AABB& mOther) const noexcept { return !(*this == mOther); }

			inline void move(const Vec2i& mOffset) noexcept { position += mOffset; }

			inline void setPosition(const Vec2i& mPosition) noexcept	{ position = mPosition; }
			inline void setX(int mX) noexcept							{ position.x = mX; }
			inline void setY(int mY) noexcept							{ position.y = mY; }
			inline void setHalfSize(const Vec2i& mHalfSize) noexcept	{ halfSize = mHalfSize; }
			inline void setSize(const Vec2i& mSize) noexcept			{ halfSize = mSize / 2; }
			inline void setWidth(int mWidth) noexcept					{ halfSize.x = mWidth / 2; }
			inline void setHeight(int mHeight) noexcept					{ halfSize.y = mHeight / 2; }

			inline const Vec2i& getPosition() const noexcept	{ return position; }
			inline int getX() const noexcept					{ return position.x; }
			inline int getY() const noexcept					{ return position.y; }
			inline int getLeft() const noexcept					{ return position.x - halfSize.x; }
			inline int getRight() const noexcept				{ return position.x + halfSize.x; }
			inline int getTop() const noexcept					{ return position.y - halfSize.y; }
			inline int getBottom() const noexcept				{ return position.y + halfSize.y; }
			inline const Vec2i& getHalfSize() const noexcept	{ return halfSize; }
			inline int getHalfWidth() const noexcept			{ return halfSize.x; }
			inline int getHalfHeight() const noexcept			{ return halfSize.y; }
			inline Vec2i getSize() const noexcept				{ return halfSize * 2; }
			inline int getWidth() const noexcept				{ return halfSize.x * 2; }
			inline int getHeight() const noexcept				{ return halfSize.y * 2; }

			template<typename T> inline Vec2<T> getVertexNW() const noexcept			{ return Vec2<T>(getLeft(), getTop()); }
			template<typename T> inline Vec2<T> getVertexNE() const noexcept			{ return Vec2<T>(getRight(), getTop()); }
			template<typename T> inline Vec2<T> getVertexSW() const noexcept			{ return Vec2<T>(getLeft(), getBottom()); }
			template<typename T> inline Vec2<T> getVertexSE() const noexcept			{ return Vec2<T>(getRight(), getBottom()); }
			template<typename T> inline Segment<T> getSegmentLeft() const noexcept		{ return {getVertexNW<T>(), getVertexSW<T>()}; }
			template<typename T> inline Segment<T> getSegmentRight() const noexcept		{ return {getVertexNE<T>(), getVertexSE<T>()}; }
			template<typename T> inline Segment<T> getSegmentTop() const noexcept		{ return {getVertexNW<T>(), getVertexNE<T>()}; }
			template<typename T> inline Segment<T> getSegmentBottom() const noexcept	{ return {getVertexSW<T>(), getVertexSE<T>()}; }

			inline bool isLeftOf(const AABB& mAABB) const noexcept	{ return getRight() <= mAABB.getLeft(); }
			inline bool isRightOf(const AABB& mAABB) const noexcept	{ return getLeft() >= mAABB.getRight(); }
			inline bool isAbove(const AABB& mAABB) const noexcept	{ return getBottom() <= mAABB.getTop(); }
			inline bool isBelow(const AABB& mAABB) const noexcept	{ return getTop() >= mAABB.getBottom(); }

			inline bool isOverlapping(const Vec2i& mPoint) const noexcept			{ return mPoint.x >= getLeft() && mPoint.x < getRight() && mPoint.y >= getTop() && mPoint.y < getBottom(); }
			inline bool isOverlapping(const Segment<int>& mSegment) const noexcept	{ return Utils::isSegmentInsersecting(mSegment, getSegmentLeft<int>()) || Utils::isSegmentInsersecting(mSegment, getSegmentRight<int>()) || Utils::isSegmentInsersecting(mSegment, getSegmentTop<int>()) || Utils::isSegmentInsersecting(mSegment, getSegmentBottom<int>()); }
			inline bool isOverlapping(const AABB& mAABB) const noexcept				{ return !isLeftOf(mAABB) && !isRightOf(mAABB) && !isAbove(mAABB) && !isBelow(mAABB); }
			inline bool contains(const Vec2i& mPoint) const noexcept				{ return isOverlapping(mPoint); }
			inline bool contains(const Segment<int>& mSegment) const noexcept		{ return contains(mSegment.start) && contains(mSegment.end); }
			inline bool contains(const AABB& mAABB) const noexcept					{ return mAABB.getLeft() >= getLeft() && mAABB.getRight() < getRight() && mAABB.getTop() >= getTop() && mAABB.getBottom() < getBottom(); }
	};
}

#endif
