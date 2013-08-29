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
			const AABB& oldShape(mBody.getOldShape());
			ssvu::sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return Utils::getOverlapArea(shape, mA->getShape()) > Utils::getOverlapArea(shape, mB->getShape()); });

			for(const auto& b : mBodiesToResolve)
			{
				bool noResolvePosition{false}, noResolveVelocity{false};
				AABB& s(b->getShape());
				const AABB& os(b->getOldShape());

				int iX{Utils::getMinIntersectionX(shape, s)}, iY{Utils::getMinIntersectionY(shape, s)};
				bool minAbs{std::abs(iX) < std::abs(iY)};
				Vec2i resolution{minAbs ? Vec2i{iX, 0} : Vec2i{0, iY}};
				mBody.onResolution({*b, b->getUserData(), {iX, iY}, resolution, noResolvePosition, noResolveVelocity});

				shape.move(resolution);

				if(noResolveVelocity) continue;

				Vec2f normal(ssvs::getNormalized(-resolution));
				float invMassA{mBody.getInvMass()}, invMassB{b->getInvMass()};


				bool oldShapeLeftOfS{oldShape.isLeftOf(s)}, oldShapeRightOfS{oldShape.isRightOf(s)};
				bool oldShapeAboveS{oldShape.isAbove(s)}, oldShapeBelowS{oldShape.isBelow(s)};
				bool oldHOverlap{!(oldShapeLeftOfS || oldShapeRightOfS)}, oldVOverlap{!(oldShapeAboveS || oldShapeBelowS)};

				float restitutionX{mBody.getRestitutionX()}, restitutionY{mBody.getRestitutionY()};
				const auto& velocity(mBody.getVelocity());

				float desiredX{velocity.x}, desiredY{velocity.y};

				if		(resolution.y < 0 && velocity.y > 0 && (oldShapeAboveS || (os.isBelow(shape) && oldHOverlap))) desiredY = (velocity.y * restitutionY);
				else if	(resolution.y > 0 && velocity.y < 0 && (oldShapeBelowS || (os.isAbove(shape) && oldHOverlap))) desiredY = (velocity.y * restitutionY);

				if		(resolution.x < 0 && velocity.x > 0 && (oldShapeLeftOfS || (os.isRightOf(shape) && oldVOverlap))) desiredX = (velocity.x * restitutionX);
				else if	(resolution.x > 0 && velocity.x < 0 && (oldShapeRightOfS || (os.isLeftOf(shape) && oldVOverlap))) desiredX = (velocity.x * restitutionX);

				Vec2f velDiff{b->getVelocity() - mBody.getVelocity()};
				float remove{ssvs::getDotProduct(velDiff, normal) + 0.4 * ((minAbs ? iX : iY) + 1.f) / mFrameTime};

				if(remove < 0)
				{
					Vec2f impulse{remove / (invMassA + invMassB) * normal};
					mBody.applyImpulse(impulse);
					b->applyImpulse(-impulse);
				}

				if(std::abs(mBody.getVelocity().x) < std::abs(desiredX)) mBody.setVelocityX(desiredX);
				if(std::abs(mBody.getVelocity().y) < std::abs(desiredY)) mBody.setVelocityY(desiredY);
			}
		}
	};

	// TODO: impulse resolver does not need iterative collision detection/response as retro resolver
	// implement a generic way to ask the spatial for collision pairs then make impulse use it
}

#endif
