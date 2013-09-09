// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_RESOLVER_IMPULSE
#define SSVSC_RESOLVER_IMPULSE

#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Global/Typedefs.h"
#include "SSVSCollision/Resolver/ResolverBase.h"

namespace ssvsc
{
	class Body;

	struct Impulse : public ResolverBase
	{
		void resolve(float, Body& mBody, std::vector<Body*>& mBodiesToResolve) override
		{
			AABB& shape(mBody.getShape());
			const AABB& oldShape(mBody.getOldShape());

			ssvu::sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return Utils::getOverlapArea(shape, mA->getShape()) > Utils::getOverlapArea(shape, mB->getShape()); });
			int resXNeg{0}, resXPos{0}, resYNeg{0}, resYPos{0};
			constexpr int tolerance{20};

			for(const auto& b : mBodiesToResolve)
			{
				int iX{Utils::getMinIntersectionX(shape, b->getShape())}, iY{Utils::getMinIntersectionY(shape, b->getShape())};

				if(std::abs(iX) < std::abs(iY))
				{
					resXNeg = std::min(resXNeg, iX);
					resXPos = std::max(resXPos, iX);
				}
				else
				{
					resYNeg = std::min(resYNeg, iY);
					resYPos = std::max(resYPos, iY);
				}
			}

			for(const auto& b : mBodiesToResolve)
			{
				const AABB& s(b->getShape());
				if(!shape.isOverlapping(s)) continue;

				int iX{Utils::getMinIntersectionX(shape, s)}, iY{Utils::getMinIntersectionY(shape, s)};
				bool noResolvePosition{false}, noResolveVelocity{false};
				Vec2i resolution{std::abs(iX) < std::abs(iY) ? Vec2i{iX, 0} : Vec2i{0, iY}};

				mBody.onResolution({*b, b->getUserData(), {iX, iY}, resolution, noResolvePosition, noResolveVelocity});

				if(!noResolvePosition) mBody.resolvePosition(resolution);
				if(noResolveVelocity) continue;

				bool oldShapeLeftOfS{oldShape.isLeftOf(s)}, oldShapeRightOfS{oldShape.isRightOf(s)};
				bool oldShapeAboveS{oldShape.isAbove(s)}, oldShapeBelowS{oldShape.isBelow(s)};
				bool oldHOverlap{!(oldShapeLeftOfS || oldShapeRightOfS)}, oldVOverlap{!(oldShapeAboveS || oldShapeBelowS)};

				const auto& velocity(mBody.getVelocity());
				const AABB& os(b->getOldShape());
				float desiredX{velocity.x}, desiredY{velocity.y};

				Vec2f normal;
				if(resolution.y < 0 && velocity.y > 0 && (oldShapeAboveS || (os.isBelow(shape) && oldHOverlap)))
				{
					if(std::abs(iY - resYNeg) < tolerance) normal.y = 1.f;
					desiredY *= mBody.getRestitutionY();
				}
				else if(resolution.y > 0 && velocity.y < 0 && (oldShapeBelowS || (os.isAbove(shape) && oldHOverlap)))
				{
					if(std::abs(iY - resYPos) < tolerance) normal.y = -1.f;
					desiredY *= mBody.getRestitutionY();
				}

				if(resolution.x < 0 && velocity.x > 0 && (oldShapeLeftOfS || (os.isRightOf(shape) && oldVOverlap)))
				{
					if(std::abs(iX - resXNeg) < tolerance) normal.x = 1.f;
					desiredX *= mBody.getRestitutionX();
				}
				else if(resolution.x > 0 && velocity.x < 0 && (oldShapeRightOfS || (os.isLeftOf(shape) && oldVOverlap)))
				{
					if(std::abs(iX - resXPos) < tolerance) normal.x = -1.f;
					desiredX *= mBody.getRestitutionX();
				}

				Vec2f velDiff{b->getVelocity() - mBody.getVelocity()};
				float velAlongNormal{ssvs::getDotProduct(velDiff, normal)};
				if(velAlongNormal > 0) continue;
				float invMassSum{mBody.getInvMass() + b->getInvMass()};
				float computedVel{velAlongNormal / invMassSum};
				Vec2f impulse{-(1.f + mBody.getRestitutionX()) * computedVel * normal.x , -(1.f + mBody.getRestitutionY()) * computedVel * normal.y};

				if(normal.y != 0)
				{
					float velTransferX{b->getVelocity().x - mBody.getVelocity().x};
					velTransferX /= invMassSum;
					if(b->getVelTransferMultX() != 0) velTransferX *= std::sqrt(mBody.getVelTransferMultX() * b->getVelTransferMultX()); else velTransferX *= 0;
					mBody.getVelTransferImpulse().x += velTransferX;

				}
				if(normal.x != 0)
				{
					float velTransferY{b->getVelocity().y - mBody.getVelocity().y};
					velTransferY /= invMassSum;
					if(b->getVelTransferMultY() != 0) velTransferY *= std::sqrt(mBody.getVelTransferMultY() * b->getVelTransferMultY()); else velTransferY *= 0;
					mBody.getVelTransferImpulse().y += velTransferY;
				}

				mBody.applyImpulse(*b, -impulse);
				b->applyImpulse(mBody, impulse);

				mBody.setVelocityX(std::abs(desiredX) * ssvu::getSign(mBody.getVelocity().x));
				mBody.setVelocityY(std::abs(desiredY) * ssvu::getSign(mBody.getVelocity().y));
			}
		}
		inline void postUpdate(World& mWorld) override
		{
			for(const auto& b : mWorld.getBodies())
			{
				b->applyImpulse(b->getVelTransferImpulse());
				ssvs::nullify(b->getVelTransferImpulse());
			}
		}
	};
}

#endif
