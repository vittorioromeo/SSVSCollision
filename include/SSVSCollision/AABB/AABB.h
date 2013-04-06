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

			void move(sf::Vector2i mOffset);

			void setPosition(sf::Vector2i mPosition);
			void setX(int mX);
			void setY(int mY);
			void setHalfSize(sf::Vector2i mHalfSize);
			void setSize(sf::Vector2i mSize);
			void setWidth(int mWidth);
			void setHeight(int mHeight);

			sf::Vector2i getPosition() const;
			int getX() const;
			int getY() const;
			int getLeft() const;
			int getRight() const;
			int getTop() const;
			int getBottom() const;
			sf::Vector2i getHalfSize() const;
			int getHalfWidth() const;
			int getHalfHeight() const;
			sf::Vector2i getSize() const;
			int getWidth() const;
			int getHeight() const;
			template<typename T> sf::Vector2<T> getNWCorner() const	{ return sf::Vector2<T>(getLeft(), getTop()); }
			template<typename T> sf::Vector2<T> getNECorner() const	{ return sf::Vector2<T>(getRight(), getTop()); }
			template<typename T> sf::Vector2<T> getSWCorner() const	{ return sf::Vector2<T>(getLeft(), getBottom()); }
			template<typename T> sf::Vector2<T> getSECorner() const	{ return sf::Vector2<T>(getRight(), getBottom()); }
			template<typename T> Segment<T> getLeftSegment() const	{ return {getNWCorner<T>(), getSWCorner<T>()}; }
			template<typename T> Segment<T> getRightSegment() const	{ return {getNECorner<T>(), getSECorner<T>()}; }
			template<typename T> Segment<T> getTopSegment() const	{ return {getNWCorner<T>(), getNECorner<T>()}; }
			template<typename T> Segment<T> getBottomSegment() const{ return {getSWCorner<T>(), getSECorner<T>()}; }

			bool isLeftOf(const AABB& mAABB) const;
			bool isRightOf(const AABB& mAABB) const;
			bool isAbove(const AABB& mAABB) const;
			bool isBelow(const AABB& mAABB) const;

			bool operator==(const AABB& mOther) const;
			bool operator!=(const AABB& mOther) const;
	};
}

#endif
