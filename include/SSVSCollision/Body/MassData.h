// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_MASSDATA
#define SSVSC_BODY_MASSDATA

namespace ssvsc
{
	struct MassData
	{
		float mass{1}, invMass{1};

		// Setters
		inline void setMass(float mMass) { mass = mMass; invMass = mMass == 0 ? 0 : 1.f / mMass; }

		// Getters
		inline float getMass() const	{ return mass; }
		inline float getInvMass() const	{ return invMass; }
	};
}

#endif
