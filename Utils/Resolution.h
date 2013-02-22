#ifndef RESOLUTION_H
#define RESOLUTION_H

namespace ssvsc
{
	namespace Utils
	{
		namespace Resolution
		{
			struct LeftOf
			{
				static sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB) { return {mB.getLeft() - mA.getRight(), 0}; }
				static sf::Vector2f getVelocity(const sf::Vector2f mVelocity) { return {0, mVelocity.y}; }
			};

			struct RightOf
			{
				static sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB) { return {mB.getRight() - mA.getLeft(), 0}; }
				static sf::Vector2f getVelocity(const sf::Vector2f mVelocity) { return {0, mVelocity.y}; }
			};

			struct Above
			{
				static sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB) { return {0, mB.getTop() - mA.getBottom()}; }
				static sf::Vector2f getVelocity(const sf::Vector2f mVelocity) { return {mVelocity.x, 0}; }
			};

			struct Below
			{
				static sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB) { return {0, mB.getBottom() - mA.getTop()}; }
				static sf::Vector2f getVelocity(const sf::Vector2f mVelocity) { return {mVelocity.x, 0}; }
			};

			struct General
			{
				static sf::Vector2i getMinIntersection(const AABB& mA, const AABB& mB) { return Utils::getMin1DIntersection(mA, mB); }
				static sf::Vector2f getVelocity(const sf::Vector2f mVelocity) { return mVelocity; }
			};
		}
	}
}

#endif // RESOLUTION_H
