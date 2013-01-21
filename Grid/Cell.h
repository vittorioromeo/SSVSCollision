#ifndef CELL_H
#define CELL_H

#include <unordered_map>
#include <vector>
#include <string>

namespace ssvsc
{
	class Body;

	class Cell
	{
		private:
			int left, right, top, bottom;
			std::unordered_map<std::string, std::vector<Body*>> bodies;

		public:
			Cell(int mLeft, int mRight, int mTop, int mBottom);			

			void add(Body* mBody);
			void del(Body* mBody);
			bool hasGroup(const std::string& mGroup);

			// Properties
			std::vector<Body*>* getQuery(const std::string& mGroup);
			int getLeft();
			int getRight();
			int getTop();
			int getBottom();
	};
}

#endif // CELL_H
