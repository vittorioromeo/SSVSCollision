// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_AABB
#define SSVSC_AABB

#include <SFML/System.hpp>
#include "SSVSCollision/Utils/Segment.h"

namespace ssvsc
{
	class AABB
	{
		private:
			sf::Vector2i position, halfSize;

		public:
			AABB(sf::Vector2i mPosition, sf::Vector2i mHalfSize);

			void move(sf::Vector2i mOffset) { position += mOffset; }

			inline void setPosition(sf::Vector2i mPosition) { position = mPosition; }
			inline void setX(int mX)				 		{ position.x = mX; }
			inline void setY(int mY)				 		{ position.y = mY; }
			inline void setHalfSize(sf::Vector2i mHalfSize)	{ halfSize = mHalfSize; }
			inline void setSize(sf::Vector2i mSize)			{ halfSize = mSize / 2; }
			inline void setWidth(int mWidth)				{ halfSize.x = mWidth / 2; }
			inline void setHeight(int mHeight)				{ halfSize.y = mHeight / 2; }

			inline sf::Vector2i getPosition() const { return position; }
			inline int getX() const					{ return position.x; }
			inline int getY() const					{ return position.y; }
			inline int getLeft() const				{ return position.x - halfSize.x; }
			inline int getRight() const				{ return position.x + halfSize.x; }
			inline int getTop() const				{ return position.y - halfSize.y; }
			inline int getBottom() const			{ return position.y + halfSize.y; }
			inline sf::Vector2i getHalfSize() const { return halfSize; }
			inline int getHalfWidth() const 		{ return halfSize.x; }
			inline int getHalfHeight() const 		{ return halfSize.y; }
			inline sf::Vector2i getSize() const 	{ return halfSize * 2; }
			inline int getWidth() const 			{ return halfSize.x * 2; }
			inline int getHeight() const 			{ return halfSize.y * 2; }

			template<typename T> inline sf::Vector2<T> getNWCorner() const	{ return sf::Vector2<T>(getLeft(), getTop()); }
			template<typename T> inline sf::Vector2<T> getNECorner() const	{ return sf::Vector2<T>(getRight(), getTop()); }
			template<typename T> inline sf::Vector2<T> getSWCorner() const	{ return sf::Vector2<T>(getLeft(), getBottom()); }
			template<typename T> inline sf::Vector2<T> getSECorner() const	{ return sf::Vector2<T>(getRight(), getBottom()); }
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
	};
}

#endif
