#ifndef CELL_H
#define CELL_H

#include <map>
#include <vector>
#include <string>

namespace ssvsc
{
	class Body;

	class Cell
	{
		private:
			int left, right, top, bottom;
			std::map<std::string, std::vector<Body*>> groupedBodies;

		public:
			Cell(int mLeft, int mRight, int mTop, int mBottom);			

			void addBody(Body* mBody);
			void delBody(Body* mBody);
			bool hasGroup(const std::string& mGroup);

			// Getters
			std::vector<Body*> getGroupedBodies(std::string mGroup);
			int getLeft();
			int getRight();
			int getTop();
			int getBottom();
	};
}

#endif // CELL_H
