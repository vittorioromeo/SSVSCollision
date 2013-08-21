// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_WORLDIMPL
#define SSVSC_WORLDIMPL

namespace ssvsc
{
	inline void World::update(float mFrameTime)
	{
		ssvu::eraseRemoveIf(bodies, &bases.isDead<Body*>);
		ssvu::eraseRemoveIf(sensors, &bases.isDead<Sensor*>);

		bases.refresh();
		for(const auto& b : bases) b->update(mFrameTime);

		spatial->refresh();
	}
}

#endif
