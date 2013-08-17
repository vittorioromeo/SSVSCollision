// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_AABB
#define SSVSC_AABB

#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Utils/Segment.h"

namespace ssvsc
{
	class AABB
	{
		private:
			Vec2i position, halfSize;

		public:
			inline AABB(Vec2i mPosition, Vec2i mHalfSize) : position{mPosition}, halfSize{mHalfSize} { }
			inline AABB(int mLeft, int mRight, int mTop, int mBottom) : position{mLeft + (mRight - mLeft) / 2, mTop + (mBottom - mTop) / 2}, halfSize{(mRight - mLeft) / 2, (mBottom - mTop) / 2} { }

			inline void move(Vec2i mOffset) { position += mOffset; }

			inline void setPosition(Vec2i mPosition)	{ position = mPosition; }
			inline void setX(int mX)					{ position.x = mX; }
			inline void setY(int mY)					{ position.y = mY; }
			inline void setHalfSize(Vec2i mHalfSize)	{ halfSize = mHalfSize; }
			inline void setSize(Vec2i mSize)			{ halfSize = mSize / 2; }
			inline void setWidth(int mWidth)			{ halfSize.x = mWidth / 2; }
			inline void setHeight(int mHeight)			{ halfSize.y = mHeight / 2; }

			inline Vec2i getPosition() const	{ return position; }
			inline int getX() const				{ return position.x; }
			inline int getY() const				{ return position.y; }
			inline int getLeft() const			{ return position.x - halfSize.x; }
			inline int getRight() const			{ return position.x + halfSize.x; }
			inline int getTop() const			{ return position.y - halfSize.y; }
			inline int getBottom() const		{ return position.y + halfSize.y; }
			inline Vec2i getHalfSize() const	{ return halfSize; }
			inline int getHalfWidth() const		{ return halfSize.x; }
			inline int getHalfHeight() const	{ return halfSize.y; }
			inline Vec2i getSize() const		{ return halfSize * 2; }
			inline int getWidth() const			{ return halfSize.x * 2; }
			inline int getHeight() const		{ return halfSize.y * 2; }

			template<typename T> inline Vec2<T> getNWCorner() const			{ return Vec2<T>(getLeft(), getTop()); }
			template<typename T> inline Vec2<T> getNECorner() const			{ return Vec2<T>(getRight(), getTop()); }
			template<typename T> inline Vec2<T> getSWCorner() const			{ return Vec2<T>(getLeft(), getBottom()); }
			template<typename T> inline Vec2<T> getSECorner() const			{ return Vec2<T>(getRight(), getBottom()); }
			template<typename T> inline Segment<T> getLeftSegment() const	{ return {getNWCorner<T>(), getSWCorner<T>()}; }
			template<typename T> inline Segment<T> getRightSegment() const	{ return {getNECorner<T>(), getSECorner<T>()}; }
			template<typename T> inline Segment<T> getTopSegment() const	{ return {getNWCorner<T>(), getNECorner<T>()}; }
			template<typename T> inline Segment<T> getBottomSegment() const	{ return {getSWCorner<T>(), getSECorner<T>()}; }

			inline bool isLeftOf(const AABB& mAABB) const	{ return getRight() <= mAABB.getLeft(); }
			inline bool isRightOf(const AABB& mAABB) const	{ return getLeft() >= mAABB.getRight(); }
			inline bool isAbove(const AABB& mAABB) const	{ return getBottom() <= mAABB.getTop(); }
			inline bool isBelow(const AABB& mAABB) const	{ return getTop() >= mAABB.getBottom(); }

			inline bool operator==(const AABB& mOther) const { return position == mOther.position && halfSize == mOther.halfSize; }
			inline bool operator!=(const AABB& mOther) const { return position != mOther.position || halfSize != mOther.halfSize; }

			inline bool isOverlapping(const AABB& mAABB) const	{ return getRight() > mAABB.getLeft() && getLeft() < mAABB.getRight() && (getBottom() > mAABB.getTop() && getTop() < mAABB.getBottom()); }
			inline bool contains(const AABB& mAABB) const		{ return !(mAABB.getLeft() < getLeft() || mAABB.getRight() >= getRight() || mAABB.getTop() < getTop() || mAABB.getBottom() >= getBottom()); }
			inline bool contains(const Vec2i mPoint) const		{ return !(mPoint.x < getLeft() || mPoint.x >= getRight() || mPoint.y < getTop() || mPoint.y >= getBottom()); }
	};
}

#endif
