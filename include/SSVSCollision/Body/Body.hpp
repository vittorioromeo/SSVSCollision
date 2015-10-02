// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY
#define SSVSC_BODY

#include "SSVSCollision/Body/CallbackInfo.hpp"
#include "SSVSCollision/Body/BodyData.hpp"
#include "SSVSCollision/Body/Groupable.hpp"
#include "SSVSCollision/Body/Base.hpp"

namespace ssvsc
{
template <typename TW>
class Body : public Base<TW>, public TW::ResolverInfoType
{
public:
    using SpatialInfoType = typename TW::SpatialInfoType;
    using DetectionInfoType = typename TW::DetectionInfoType;
    using ResolverType = typename TW::ResolverType;
    using ResolverInfoType = typename TW::ResolverInfoType;
    using ResolutionInfoType = typename TW::ResolutionInfoType;

    friend TW;
    friend SpatialInfoType;
    friend ResolverType;
    friend ResolverInfoType;

protected:
    BodyData data;
    std::vector<Body*> toResolve;
    void* userData{nullptr};
    bool mustInit{true};

    inline void integrate(FT mFT) noexcept
    {
        data.velocity += getAcceleration() * mFT;
        getShape().move(Vec2i(getVelocity() * mFT));
        ssvs::nullify(data.acceleration);
    }

    inline void update(FT mFT)
    {
        if(mustInit) {
            this->spatialInfo.template init<BodyTag>();
            mustInit = false;
        }

        ssvs::nullify(data.lastResolution);

        this->onPreUpdate();

        if(isStatic()) {
            this->spatialInfo.template preUpdate<BodyTag>();
            return;
        }
        if(this->outOfBounds) {
            onOutOfBounds();
            this->outOfBounds = false;
            return;
        }
        data.oldShape = getShape();
        data.oldVelocity = getVelocity();
        integrate(mFT);
        this->spatialInfo.template preUpdate<BodyTag>();

        toResolve.clear();
        this->spatialInfo.template handleCollisions<BodyTag>(mFT);

        this->world.resolver.resolve(*this, toResolve);
        if(getOldShape() != getShape()) this->spatialInfo.invalidate();

        this->spatialInfo.postUpdate();
        onPostUpdate();
    }

    inline void handleCollision(FT mFT, Body* mBody)
    {
        if(mBody == this || !this->mustCheck(*mBody) ||
           !getShape().isOverlapping(mBody->getShape()))
            return;

        this->onDetection({*mBody, mBody->getUserData(), mFT});
        mBody->onDetection({*this, userData, mFT});

        if(mustResolveAgainst(*mBody)) toResolve.emplace_back(mBody);
    }

public:
    using ResolverInfoType::ResolverInfoType;

    ssvu::Delegate<void()> onPostUpdate, onOutOfBounds;
    ssvu::Delegate<void(const ResolutionInfoType&)> onResolution;

    inline Body(TW& mWorld, bool mIsStatic, const Vec2i& mPos,
    const Vec2i& mSize) noexcept : Base<TW>{mWorld},
                                   data{mIsStatic, mPos, mSize}
    {
    }
    inline ~Body() noexcept { destroy(); }
    inline void destroy()
    {
        this->spatialInfo.template destroy<BodyTag>();
        this->world.delBody(this);
    }

    inline void applyAccel(const Vec2f& mAccel) noexcept
    {
        data.acceleration += mAccel;
    }
    inline void resolvePosition(const Vec2i& mOffset) noexcept
    {
        data.shape.move(mOffset);
        data.lastResolution += mOffset;
    }

    inline void setPosition(const Vec2i& mPos)
    {
        data.oldShape = getShape();
        data.shape.setPosition(mPos);
        this->spatialInfo.invalidate();
    }
    inline void setX(int mX)
    {
        data.oldShape = getShape();
        data.shape.setX(mX);
        this->spatialInfo.invalidate();
    }
    inline void setY(int mY)
    {
        data.oldShape = getShape();
        data.shape.setY(mY);
        this->spatialInfo.invalidate();
    }
    inline void setSize(const Vec2i& mSize)
    {
        data.shape.setSize(mSize);
        this->spatialInfo.invalidate();
    }
    inline void setHalfSize(const Vec2i& mSize)
    {
        data.shape.setHalfSize(mSize);
        this->spatialInfo.invalidate();
    }
    inline void setWidth(int mWidth)
    {
        data.shape.setWidth(mWidth);
        this->spatialInfo.invalidate();
    }
    inline void setHeight(int mHeight)
    {
        data.shape.setHeight(mHeight);
        this->spatialInfo.invalidate();
    }
    inline void setStatic(bool mStatic)
    {
        data._static = mStatic;
        this->spatialInfo.invalidate();
    }
    inline void setVelocity(const Vec2f& mVel) noexcept
    {
        data.velocity = mVel;
    }
    inline void setAcceleration(const Vec2f& mAccel) noexcept
    {
        data.acceleration = mAccel;
    }
    inline void setUserData(void* mUserData) noexcept { userData = mUserData; }
    inline void setVelocityX(float mX) noexcept { data.velocity.x = mX; }
    inline void setVelocityY(float mY) noexcept { data.velocity.y = mY; }
    inline void setResolve(bool mResolve) noexcept { data.resolve = mResolve; }
    inline void setMass(float mMass) noexcept { data.setMass(mMass); }
    inline void setRestitutionX(float mX) noexcept { data.restitution.x = mX; }
    inline void setRestitutionY(float mY) noexcept { data.restitution.y = mY; }

    inline AABB& getShape() noexcept { return data.shape; }
    inline AABB& getOldShape() noexcept { return data.oldShape; }
    inline const AABB& getShape() const noexcept { return data.shape; }
    inline const AABB& getOldShape() const noexcept { return data.oldShape; }
    inline const auto& getPosition() const noexcept
    {
        return getShape().getPosition();
    }
    inline const auto& getVelocity() const noexcept { return data.velocity; }
    inline const auto& getOldPosition() const noexcept
    {
        return getOldShape().getPosition();
    }
    inline const auto& getOldVelocity() const noexcept
    {
        return data.oldVelocity;
    }
    inline const auto& getAcceleration() const noexcept
    {
        return data.acceleration;
    }
    inline auto getSize() const noexcept { return getShape().getSize(); }
    inline auto getMass() const noexcept { return isStatic() ? 0 : data.mass; }
    inline auto getInvMass() const noexcept
    {
        return isStatic() ? 0 : data.invMass;
    }
    inline int getWidth() const noexcept { return getShape().getWidth(); }
    inline int getHeight() const noexcept { return getShape().getHeight(); }
    inline bool isStatic() const noexcept { return data._static; }
    inline bool hasMovedLeft() const noexcept
    {
        return getShape().getX() < getOldShape().getX();
    }
    inline bool hasMovedRight() const noexcept
    {
        return getShape().getX() > getOldShape().getX();
    }
    inline bool hasMovedUp() const noexcept
    {
        return getShape().getY() < getOldShape().getY();
    }
    inline bool hasMovedDown() const noexcept
    {
        return getShape().getY() > getOldShape().getY();
    }
    inline bool getResolve() const noexcept { return data.resolve; }
    inline const auto& getLastResolution() const noexcept
    {
        return data.lastResolution;
    }
    inline float getRestitutionX() const noexcept { return data.restitution.x; }
    inline float getRestitutionY() const noexcept { return data.restitution.y; }

    inline void* getUserData() const noexcept { return userData; }
    template <typename T>
    inline T getUserData() const noexcept
    {
        return static_cast<T>(userData);
    }

    inline bool mustResolveAgainst(const Body& mBody) const noexcept
    {
        return getResolve() && !this->mustIgnoreResolution(mBody);
    }
};
}

#include "SSVSCollision/Body/Sensor.hpp"

#endif
