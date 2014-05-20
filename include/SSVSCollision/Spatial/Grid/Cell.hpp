// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVSC_SPATIAL_GRID_CELL
#define SSVSC_SPATIAL_GRID_CELL

namespace ssvsc
{
	template<typename TW> class Cell
	{
		public:
			using BaseType = Base<TW>;
			using BodyType = Body<TW>;

		private:
			std::vector<BodyType*> bodies;

		public:
			inline void add(BaseType* mBase, BodyTag) { bodies.emplace_back(reinterpret_cast<BodyType*>(mBase)); }
			inline void del(BaseType* mBase, BodyTag) { ssvu::eraseRemove(bodies, reinterpret_cast<BodyType*>(mBase)); }
			inline void add(BaseType*, SensorTag) { }
			inline void del(BaseType*, SensorTag) { }

			inline const decltype(bodies)& getBodies() const noexcept { return bodies; }
	};
}

#endif
