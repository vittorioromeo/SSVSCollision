// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include <SSVUtils/SSVUtils.h>
#include <SSVStart/SSVStart.h>
#include "SSVSCollision/Resolver/Impulse/Impulse.h"
#include "SSVSCollision/Utils/Utils.h"
#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Global/Typedefs.h"

using namespace std;
using namespace ssvsc::Utils;
using namespace ssvu;
using namespace ssvs::Utils;

namespace ssvsc
{
	void Impulse::resolve(Body& mBody, vector<Body*>& mBodiesToResolve)
	{
		AABB& shape(mBody.getShape());
		sort(mBodiesToResolve, [&](Body* mA, Body* mB){ return getOverlapArea(shape, mA->getShape()) > getOverlapArea(shape, mB->getShape()); });

		for(const auto& b : mBodiesToResolve)
		{
			bool noResolvePosition{false}, noResolveVelocity{false};
			AABB& s(b->getShape());

			auto intersection(getMinIntersection(shape, s));
			Vec2i resolution{getMin1DIntersection(shape, s)};
			mBody.onResolution({*b, b->getUserData(), intersection, resolution, noResolvePosition, noResolveVelocity});

			Vec2f normal(getNormalized(-resolution));
			float invMassA{mBody.getInvMass()}, invMassB{b->getInvMass()};

			if(noResolveVelocity) continue;

			Vec2f rv{b->getVelocity() - mBody.getVelocity()};
			float velAlongNormal{getDotProduct(rv, normal)};
			if(velAlongNormal <= 0)
			{
				float restitutionA{0.8f}, restitutionB{0.8f};
				if(b->isStatic()) restitutionB = 0.1f;
				float restitution{min(restitutionA, restitutionB)};

				float impulseMultiplier{(1.0f + restitution) * -velAlongNormal};
				impulseMultiplier /= invMassA + invMassB;
				Vec2f impulse{normal * impulseMultiplier};

				mBody.applyImpulse(-impulse);
				b->applyImpulse(impulse);
			}

			if(noResolvePosition) continue;

			const float k_slop{0.05f}, percent{0.4f};
			Vec2f correction{(max(getMagnitude(Vec2f(resolution)) - k_slop, 0.0f) / (invMassA + invMassB) * percent) * normal};
			shape.move(-Vec2i(invMassA * correction));
			s.move(Vec2i(invMassB * correction));
		}
	}
}

