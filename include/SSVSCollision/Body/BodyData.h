// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_BODYDATA
#define SSVSC_BODY_BODYDATA

namespace ssvsc
{
	class MassData
	{
		private:
			float mass{1.f}, invMass{1.f};

		public:
			inline void setMass(float mMass) noexcept	{ mass = mMass; invMass = (mMass == 0 ? 0 : (1.f / mMass)); }
			inline float getMass() const noexcept		{ return mass; }
			inline float getInvMass() const noexcept	{ return invMass; }
	};

	class RestitutionData
	{
		private:
			float restitutionX{0.f}, restitutionY{0.f};

		public:
			inline void setRestitutionX(float mRestX) noexcept	{ restitutionX = mRestX; }
			inline void setRestitutionY(float mRestY) noexcept	{ restitutionY = mRestY; }
			inline float getRestitutionX() const noexcept		{ return restitutionX; }
			inline float getRestitutionY() const noexcept		{ return restitutionY; }
	};
}

#endif
