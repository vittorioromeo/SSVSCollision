// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "World/World.h"
#include "Body/Body.h"
#include "Utils/Utils.h"
#include "Resolver/ResolverBase.h"
#include "Spatial/SpatialBase.h"

using namespace std;
using namespace ssvsc::Utils;

namespace ssvsc
{
	World::World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver{&mResolver}, spatial{&mSpatial} { }
	World::~World() { delete resolver; delete spatial; }

	Body& World::create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic)
	{
		return memoryManager.create(*this, mIsStatic, mPosition, mSize);
	}

	void World::del(Body* mBody) { memoryManager.del(mBody); }
	void World::update(float mFrameTime)
	{
		memoryManager.cleanUp();
		for(auto& body : memoryManager.getItems()) body->update(mFrameTime);
	}

	ResolverBase& World::getResolver() 	{ return *resolver; }
	SpatialBase& World::getSpatial() 	{ return *spatial; }
}
