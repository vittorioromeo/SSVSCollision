// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef AABB_H
#define AABB_H

#include <SFML/Graphics.hpp>

namespace ssvsc
{
	class AABB
	{
		private:
			sf::Vector2i position, halfSize;

		public:
			AABB(sf::Vector2i mPosition, sf::Vector2i mHalfSize);

			void move(sf::Vector2i mOffset);

			void setX(int mX);
			void setY(int mY);
			void setPosition(sf::Vector2i mPosition);
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
			int getHalfWidth() const;
			int getHalfHeight() const;
			int getWidth() const;
			int getHeight() const;
			sf::Vector2i getSize() const;
			bool isLeftOf(const AABB& mAABB) const;
			bool isRightOf(const AABB& mAABB) const;
			bool isAbove(const AABB& mAABB) const;
			bool isBelow(const AABB& mAABB) const;

			bool operator==(const AABB& mOther) const;
	};
}

#endif // AABB_H
