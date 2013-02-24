// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "World.h"
#include "Body/Body.h"
#include "Utils/Utils.h"
#include "Resolver/ResolverBase.h"
#include "Spatial/SpatialBase.h"

using namespace std;
using namespace ssvsc::Utils;

namespace ssvsc
{
	World::World(ResolverBase& mResolver, SpatialBase& mSpatial) : resolver{&mResolver}, spatial{&mSpatial} { bodiesToDel.set_empty_key(nullptr); }
	World::~World()
	{
		for(auto& body : bodies) delete body;
		delete resolver;
		delete spatial;
	}

	Body& World::create(sf::Vector2i mPosition, sf::Vector2i mSize, bool mIsStatic)
	{
		Body* result{new Body{*this, mIsStatic, mPosition, mSize}};
		add(result);
		return *result;
	}

	void World::add(Body* mBody) { bodies.push_back(mBody); }
	void World::del(Body* mBody) { bodiesToDel.insert(mBody); }
	void World::update(float mFrameTime)
	{
		for(auto& body : bodiesToDel) { eraseRemove(bodies, body); delete body; }
		bodiesToDel.clear();

		for(auto& body : bodies) body->update(mFrameTime);
	}

	ResolverBase& World::getResolver() { return *resolver; }
	SpatialBase& World::getSpatial() { return *spatial; }
}
