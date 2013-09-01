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
		void resolve(float mFrameTime, Body& mBody, std::vector<Body*>& mBodiesToResolve) override
		{
			AABB& shape(mBody.getShape());
			int resXNeg{0}, resXPos{0}, resYNeg{0}, resYPos{0};

			for(const auto& b : mBodiesToResolve)
			{
				int iX{Utils::getMinIntersectionX(shape, b->getShape())}, iY{Utils::getMinIntersectionY(shape, b->getShape())};

				if(std::abs(iX) < std::abs(iY))
				{
					if(iX < 0) resXNeg = std::min(resXNeg, iX);
					else if(iX > 0) resXPos = std::max(resXPos, iX);
				}
				else
				{
					if(iY < 0) resYNeg = std::min(resYNeg, iY);
					else if(iY > 0) resYPos = std::max(resYPos, iY);
				}
			}

			const AABB& oldShape(mBody.getOldShape());
			ssvu::sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return Utils::getOverlapArea(shape, mA->getShape()) > Utils::getOverlapArea(shape, mB->getShape()); });
			for(const auto& b : mBodiesToResolve)
			{
				const AABB& s(b->getShape());

				int iX{Utils::getMinIntersectionX(shape, s)}, iY{Utils::getMinIntersectionY(shape, s)};
				bool noResolvePosition{false}, noResolveVelocity{false};
				Vec2i resolution{std::abs(iX) < std::abs(iY) ? Vec2i{iX, 0} : Vec2i{0, iY}};

				mBody.onResolution({*b, b->getUserData(), {iX, iY}, resolution, noResolvePosition, noResolveVelocity});

				if(!noResolvePosition) shape.move(resolution);
				if(noResolveVelocity) continue;

				bool oldShapeLeftOfS{oldShape.isLeftOf(s)}, oldShapeRightOfS{oldShape.isRightOf(s)};
				bool oldShapeAboveS{oldShape.isAbove(s)}, oldShapeBelowS{oldShape.isBelow(s)};
				bool oldHOverlap{!(oldShapeLeftOfS || oldShapeRightOfS)}, oldVOverlap{!(oldShapeAboveS || oldShapeBelowS)};

				const auto& velocity(mBody.getVelocity());
				const AABB& os(b->getOldShape());
				float desiredX{velocity.x}, desiredY{velocity.y};

				Vec2f normal;
				if (resolution.y < 0 && velocity.y > 0 && (oldShapeAboveS || (os.isBelow(shape) && oldHOverlap)))
				{
					if(iY == resYNeg) normal.y = 1.f;
					desiredY *= mBody.getRestitutionY();
				}
				else if (resolution.y > 0 && velocity.y < 0 && (oldShapeBelowS || (os.isAbove(shape) && oldHOverlap)))
				{
					if(iY == resYPos) normal.y = -1.f;
					desiredY *= mBody.getRestitutionY();
				}

				if (resolution.x < 0 && velocity.x > 0 && (oldShapeLeftOfS || (os.isRightOf(shape) && oldVOverlap)))
				{
					if(iX == resXNeg) normal.x = 1.f;
					desiredX *= mBody.getRestitutionX();
				}
				else if (resolution.x > 0 && velocity.x < 0 && (oldShapeRightOfS || (os.isLeftOf(shape) && oldVOverlap)))
				{
					if(iX == resXPos) normal.x = -1.f;
					desiredX *= mBody.getRestitutionX();
				}

				Vec2f velDiff{b->getVelocity() - mBody.getVelocity()};
				float velAlongNormal = ssvs::getDotProduct(velDiff, normal);
				if(velAlongNormal > 0) continue;
				float computedVel{velAlongNormal / (mBody.getInvMass() + b->getInvMass())};
				Vec2f impulse{-(1.f + mBody.getRestitutionX()) * computedVel * normal.x , -(1.f + mBody.getRestitutionY()) * computedVel * normal.y};

				mBody.applyImpulse(-impulse);
				b->applyImpulse(impulse);

				mBody.setVelocityX(std::abs(desiredX) * ssvu::getSign(mBody.getVelocity().x));
				mBody.setVelocityY(std::abs(desiredY) * ssvu::getSign(mBody.getVelocity().y));
			}
		}
	};
}

#endif
