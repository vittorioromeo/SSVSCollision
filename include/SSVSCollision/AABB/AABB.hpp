// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_AABB
#define SSVSC_AABB

namespace ssvsc
{
class AABB
{
private:
    Vec2i position, halfSize;

public:
    inline AABB(const Vec2i& mPosition, const Vec2i& mHalfSize) noexcept
    : position{mPosition},
      halfSize{mHalfSize}
    {
    }
    inline AABB(int mLeft, int mRight, int mTop, int mBottom) noexcept
    : position{mLeft + (mRight - mLeft) / 2, mTop + (mBottom - mTop) / 2},
      halfSize{(mRight - mLeft) / 2, (mBottom - mTop) / 2}
    {
    }

    inline bool operator==(const AABB& mRhs) const noexcept
    {
        return position == mRhs.position && halfSize == mRhs.halfSize;
    }
    inline bool operator!=(const AABB& mRhs) const noexcept
    {
        return !(*this == mRhs);
    }

    inline void move(const Vec2i& mOffset) noexcept { position += mOffset; }

    inline void setPosition(const Vec2i& mPosition) noexcept
    {
        position = mPosition;
    }
    inline void setX(int mX) noexcept { position.x = mX; }
    inline void setY(int mY) noexcept { position.y = mY; }
    inline void setHalfSize(const Vec2i& mHalfSize) noexcept
    {
        halfSize = mHalfSize;
    }
    inline void setSize(const Vec2i& mSize) noexcept { halfSize = mSize / 2; }
    inline void setWidth(int mWidth) noexcept { halfSize.x = mWidth / 2; }
    inline void setHeight(int mHeight) noexcept { halfSize.y = mHeight / 2; }

    inline const auto& getPosition() const noexcept { return position; }
    inline int getX() const noexcept { return position.x; }
    inline int getY() const noexcept { return position.y; }
    inline int getLeft() const noexcept { return position.x - halfSize.x; }
    inline int getRight() const noexcept { return position.x + halfSize.x; }
    inline int getTop() const noexcept { return position.y - halfSize.y; }
    inline int getBottom() const noexcept { return position.y + halfSize.y; }
    inline const auto& getHalfSize() const noexcept { return halfSize; }
    inline int getHalfWidth() const noexcept { return halfSize.x; }
    inline int getHalfHeight() const noexcept { return halfSize.y; }
    inline auto getSize() const noexcept { return halfSize * 2; }
    inline int getWidth() const noexcept { return halfSize.x * 2; }
    inline int getHeight() const noexcept { return halfSize.y * 2; }

    template <typename T>
    inline auto getVertexNW() const noexcept
    {
        return Vec2<T>(getLeft(), getTop());
    }
    template <typename T>
    inline auto getVertexNE() const noexcept
    {
        return Vec2<T>(getRight(), getTop());
    }
    template <typename T>
    inline auto getVertexSW() const noexcept
    {
        return Vec2<T>(getLeft(), getBottom());
    }
    template <typename T>
    inline auto getVertexSE() const noexcept
    {
        return Vec2<T>(getRight(), getBottom());
    }
    template <typename T>
    inline auto getSegmentLeft() const noexcept
    {
        return Segment<T>{getVertexNW<T>(), getVertexSW<T>()};
    }
    template <typename T>
    inline auto getSegmentRight() const noexcept
    {
        return Segment<T>{getVertexNE<T>(), getVertexSE<T>()};
    }
    template <typename T>
    inline auto getSegmentTop() const noexcept
    {
        return Segment<T>{getVertexNW<T>(), getVertexNE<T>()};
    }
    template <typename T>
    inline auto getSegmentBottom() const noexcept
    {
        return Segment<T>{getVertexSW<T>(), getVertexSE<T>()};
    }

    inline bool isLeftOf(const AABB& mX) const noexcept
    {
        return getRight() <= mX.getLeft();
    }
    inline bool isRightOf(const AABB& mX) const noexcept
    {
        return getLeft() >= mX.getRight();
    }
    inline bool isAbove(const AABB& mX) const noexcept
    {
        return getBottom() <= mX.getTop();
    }
    inline bool isBelow(const AABB& mX) const noexcept
    {
        return getTop() >= mX.getBottom();
    }

    inline bool isOverlapping(const Vec2i& mX) const noexcept
    {
        return mX.x >= getLeft() && mX.x < getRight() && mX.y >= getTop() &&
               mX.y < getBottom();
    }
    inline bool isOverlapping(const Segment<int>& mX) const noexcept
    {
        return Utils::isSegmentInsersecting(mX, getSegmentLeft<int>()) ||
               Utils::isSegmentInsersecting(mX, getSegmentRight<int>()) ||
               Utils::isSegmentInsersecting(mX, getSegmentTop<int>()) ||
               Utils::isSegmentInsersecting(mX, getSegmentBottom<int>());
    }
    inline bool isOverlapping(const AABB& mX) const noexcept
    {
        return !isLeftOf(mX) && !isRightOf(mX) && !isAbove(mX) && !isBelow(mX);
    }
    inline bool contains(const Vec2i& mX) const noexcept
    {
        return isOverlapping(mX);
    }
    inline bool contains(const Segment<int>& mX) const noexcept
    {
        return contains(mX.start) && contains(mX.end);
    }
    inline bool contains(const AABB& mX) const noexcept
    {
        return mX.getLeft() >= getLeft() && mX.getRight() < getRight() &&
               mX.getTop() >= getTop() && mX.getBottom() < getBottom();
    }
};
}

#endif
