// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILSAABB
#define SSVSC_UTILSAABB

namespace ssvsc
{
	namespace Utils
	{
		template<typename T> inline constexpr T getMinAbs(const T& mA, const T& mB) noexcept { return std::abs(mA) < std::abs(mB) ? mA : mB; }

		inline int getMinIntersectionX(const AABB& mA, const AABB& mB) noexcept { return getMinAbs(mB.getLeft() - mA.getRight(), mB.getRight() - mA.getLeft()); }
		inline int getMinIntersectionY(const AABB& mA, const AABB& mB) noexcept { return getMinAbs(mB.getTop() - mA.getBottom(), mB.getBottom() - mA.getTop()); }
		inline auto getMin1DIntersection(const AABB& mA, const AABB& mB) noexcept
		{
			int iX{getMinIntersectionX(mA, mB)}, iY{getMinIntersectionY(mA, mB)};
			return abs(iX) < abs(iY) ? Vec2i{iX, 0} : Vec2i{0, iY};
		}
		inline auto getMinIntersection(const AABB& mA, const AABB& mB) noexcept	{ return Vec2i{getMinIntersectionX(mA, mB), getMinIntersectionY(mA, mB)}; }
		inline int getOverlapX(const AABB& mA, const AABB& mB) noexcept			{ return mA.getLeft() < mB.getLeft() ? mA.getRight() - mB.getLeft() : mB.getRight() - mA.getLeft(); }
		inline int getOverlapY(const AABB& mA, const AABB& mB) noexcept			{ return mA.getTop() < mB.getTop() ? mA.getBottom() - mB.getTop() : mB.getBottom() - mA.getTop(); }
		inline int getOverlapArea(const AABB& mA, const AABB& mB) noexcept		{ return getOverlapX(mA, mB) * getOverlapY(mA, mB); }
	}
}

#endif
