// Copyright (c) 2013-2015 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_RESOLVER_RETRO
#define SSVSC_RESOLVER_RETRO

namespace ssvsc
{
    template <typename TW>
    struct RetroInfo
    {
    public:
        using BodyType = Body<TW>;
        using ResolverType = typename TW::ResolverType;
        friend ResolverType;

    protected:
        inline auto& getBody() noexcept
        {
            return ssvu::castUp<BodyType>(*this);
        }
        inline const auto& getBody() const noexcept
        {
            return ssvu::castUp<BodyType>(*this);
        }
    };

    template <typename TW>
    struct Retro
    {
        using BodyType = Body<TW>;
        using ResolverInfoType = RetroInfo<TW>;

        inline void resolve(
            BodyType& mBody, std::vector<BodyType*>& mToResolve) const
        {
            AABB& shape(mBody.getShape());
            const AABB& oldShape(mBody.getOldShape());
            ssvu::sort(mToResolve, [&shape](BodyType* mA, BodyType* mB)
                {
                    return Utils::getOverlapArea(shape, mA->getShape()) >
                           Utils::getOverlapArea(shape, mB->getShape());
                });

            for(const auto& b : mToResolve)
            {
                const AABB& s(b->getShape());
                if(!shape.isOverlapping(s)) continue;

                int iX{Utils::getMinIntersectionX(shape, s)},
                    iY{Utils::getMinIntersectionY(shape, s)};
                Vec2i resolution{
                    std::abs(iX) < std::abs(iY) ? Vec2i{iX, 0} : Vec2i{0, iY}};
                bool noResolvePosition{false}, noResolveVelocity{false};
                mBody.onResolution({*b, b->getUserData(), resolution,
                    noResolvePosition, noResolveVelocity});

                if(!noResolvePosition) mBody.resolvePosition(resolution);
                if(noResolveVelocity) continue;

                // Remember that shape has moved now
                bool oldShapeLeftOfS{oldShape.isLeftOf(s)},
                    oldShapeRightOfS{oldShape.isRightOf(s)};
                bool oldShapeAboveS{oldShape.isAbove(s)},
                    oldShapeBelowS{oldShape.isBelow(s)};
                bool oldHOverlap{!(oldShapeLeftOfS || oldShapeRightOfS)},
                    oldVOverlap{!(oldShapeAboveS || oldShapeBelowS)};

                // TODO: consider when two different bodies with two different
                // rest.
                // collide
                const auto& vel(mBody.getVelocity());
                const AABB& os(b->getOldShape());

                // TODO: benchmark multiplying by bool instead of if?
                if((resolution.y < 0 && vel.y > 0 &&
                       (oldShapeAboveS ||
                           (os.isBelow(shape) && oldHOverlap))) ||
                    (resolution.y > 0 && vel.y < 0 &&
                        (oldShapeBelowS || (os.isAbove(shape) && oldHOverlap))))
                    mBody.setVelocityY(vel.y * -mBody.getRestitutionY());

                if((resolution.x < 0 && vel.x > 0 &&
                       (oldShapeLeftOfS ||
                           (os.isRightOf(shape) && oldVOverlap))) ||
                    (resolution.x > 0 && vel.x < 0 &&
                        (oldShapeRightOfS ||
                            (os.isLeftOf(shape) && oldVOverlap))))
                    mBody.setVelocityX(vel.x * -mBody.getRestitutionX());
            }
        }
        inline void postUpdate(TW&) const noexcept {}
    };
}

#endif
