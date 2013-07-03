// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SENSOR
#define SSVSC_SENSOR

#include <SFML/System.hpp>
#include <SSVUtils/SSVUtils.h>
#include "SSVSCollision/AABB/AABB.h"
#include "SSVSCollision/Body/CallbackInfo.h"
#include "SSVSCollision/Body/GroupData.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"
#include "SSVSCollision/Body/Base.h"

namespace ssvsc
{
	class World;

	class Sensor : public Base
	{
		private:
			AABB shape;
			GroupData groupData;

		public:
			ssvu::Delegate<void> onPreUpdate;
			ssvu::Delegate<void, DetectionInfo> onDetection;

			Sensor(World& mWorld, sf::Vector2i mPosition, sf::Vector2i mSize);
			~Sensor();

			void addGroupsToCheck(const std::vector<std::string>& mGroups);

			inline Type getType() override { return Type::Sensor; }
			void update(float mFrameTime) override;
			void destroy() override;

			inline void setPosition(sf::Vector2i mPosition)
			{
				if(mPosition != shape.getPosition()) spatialInfo.invalidate();
				shape.setPosition(mPosition);
			}

			inline AABB& getShape() override								{ return shape; }
			inline AABB& getOldShape() override								{ return shape; }
			inline const std::vector<int>& getGroupUidsToCheck() override	{ return groupData.getUidsToCheck(); }
	};
}

#endif
