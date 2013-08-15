// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_MASSDATA
#define SSVSC_BODY_MASSDATA

namespace ssvsc
{
	class MassData
	{
		private:
			float mass{1.f}, invMass{1.f};

		public:
			inline void setMass(float mMass)	{ mass = mMass; invMass = mMass == 0 ? 0 : 1.f / mMass; }
			inline float getMass() const		{ return mass; }
			inline float getInvMass() const		{ return invMass; }
	};
}

#endif
