// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_BODY_BODYDATA
#define SSVSC_BODY_BODYDATA

namespace ssvsc
{
	struct BodyData
	{
		AABB shape, oldShape;
		Vec2f velocity, oldVelocity, acceleration, restitution;
		Vec2i lastResolution;
		float mass{1.f}, invMass{1.f};
		bool _static, resolve{true};

		inline BodyData(bool mIsStatic, const Vec2i& mPos, const Vec2i& mSize) noexcept : shape{mPos, mSize / 2}, oldShape{shape}, _static{mIsStatic} { }

		inline void setMass(float mMass) noexcept { mass = mMass; invMass = (mMass == 0 ? 0 : (1.f / mMass)); }
	};
}

#endif
