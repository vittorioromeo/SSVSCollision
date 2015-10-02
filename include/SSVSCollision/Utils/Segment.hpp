// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_UTILS_SEGMENT
#define SSVSC_UTILS_SEGMENT

namespace ssvsc
{
template <typename T>
struct Segment
{
    Vec2<T> start, end;

    inline Segment() noexcept {}
    inline Segment(const Vec2<T>& mStart, const Vec2<T>& mEnd) noexcept
    : start{mStart},
      end{mEnd}
    {
    }

    inline auto getPointCrossProduct(const Vec2<T>& mPoint) const noexcept
    {
        return ((end.x - start.x) * (mPoint.y - start.y) -
                (end.y - start.y) * (mPoint.x - start.x));
    }
    inline bool isPointLeft(const Vec2<T>& mPoint) const noexcept
    {
        return getPointCrossProduct(mPoint) > 0;
    }
    inline bool isPointRight(const Vec2<T>& mPoint) const noexcept
    {
        return getPointCrossProduct(mPoint) < 0;
    }
};
}

#endif
