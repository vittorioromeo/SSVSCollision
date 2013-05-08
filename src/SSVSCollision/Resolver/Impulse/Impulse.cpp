// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SFML/System.hpp>
#include <SSVUtils/SSVUtils.h>
#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Resolver/Impulse/Impulse.h"
#include "SSVSCollision/Utils/Utils.h"

using namespace std;
using namespace sf;
using namespace ssvsc::Utils;
using namespace ssvu;
using namespace ssvs::Utils;

namespace ssvsc
{
	void Impulse::resolve(Body& mBody, std::vector<Body*>& mBodiesToResolve)
	{
		AABB& shape(mBody.getShape());
		sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return getOverlapArea(shape, mA->getShape()) > getOverlapArea(shape, mB->getShape()); });

		for(auto& b : mBodiesToResolve)
		{
			bool noResolvePosition{false}, noResolveVelocity{false};
			AABB& s(b->getShape());

			auto intersection(getMinIntersection(shape, s));
			Vector2i resolution{getMin1DIntersection(shape, s)};
			mBody.onResolution({*b, b->getUserData(), intersection, resolution, noResolvePosition, noResolveVelocity});

			Vector2f normal(getNormalized(-resolution));
			float invMassA{mBody.getInvMass()}, invMassB{b->getInvMass()};

			if(noResolveVelocity) continue;

			Vector2f rv{b->getVelocity() - mBody.getVelocity()};
			float velAlongNormal{getDotProduct(rv, normal)};
			if(velAlongNormal <= 0)
			{
				float restitutionA{0.8f}, restitutionB{0.8f};
				if(b->isStatic()) restitutionB = 0.1f;
				float restitution{min(restitutionA, restitutionB)};

				float impulseMultiplier{(1 + restitution) * -velAlongNormal};
				impulseMultiplier /= invMassA + invMassB;
				Vector2f impulse{normal * impulseMultiplier};

				mBody.setVelocity(mBody.getVelocity() - invMassA * impulse);
				b->setVelocity(b->getVelocity() + invMassB * impulse);
			}
			if(noResolvePosition) continue;

			const float k_slop{0.05f};
			const float percent{0.98f};
			Vector2f correction{(max(getMagnitude(Vector2f(resolution)) - k_slop, 0.0f) / (invMassA + invMassB) * percent) * normal};
			shape.move(-Vector2i(invMassA * correction));
			s.move(Vector2i(invMassB * correction));
		}
	}
}

