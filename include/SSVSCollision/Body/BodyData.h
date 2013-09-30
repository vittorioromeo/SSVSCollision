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
			float x{0.f}, y{0.f};

		public:
			inline void setRestitutionX(float mX) noexcept	{ x = mX; }
			inline void setRestitutionY(float mY) noexcept	{ y = mY; }
			inline float getRestitutionX() const noexcept	{ return x; }
			inline float getRestitutionY() const noexcept	{ return y; }
	};
}

#endif
