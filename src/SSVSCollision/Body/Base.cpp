// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Body/Base.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/World/World.h"

using namespace std;

namespace ssvsc
{
	Base::Base(World& mWorld) : world(mWorld), spatialInfo(world.getSpatial().createSpatialInfo(*this)) { }
	Base::~Base() { spatialInfo.destroy(); }
}
