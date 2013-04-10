// Copyright (c) 2013 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#include "SSVSCollision/Body/Body.h"
#include "SSVSCollision/Spatial/SpatialInfoBase.h"

using namespace std;
using namespace sf;

namespace ssvsc
{
	void Body::setPosition(Vector2i mPosition) 	{ oldShape = shape; shape.setPosition(mPosition); spatialInfo.invalidate(); }
	void Body::setX(int mX)				 		{ oldShape = shape; shape.setX(mX); spatialInfo.invalidate(); }
	void Body::setY(int mY)				 		{ oldShape = shape; shape.setY(mY); spatialInfo.invalidate(); }
	void Body::setSize(Vector2i mSize)			{ shape.setSize(mSize); spatialInfo.invalidate(); }
	void Body::setWidth(int mWidth)				{ shape.setWidth(mWidth); spatialInfo.invalidate(); }
	void Body::setHeight(int mHeight)			{ shape.setHeight(mHeight); spatialInfo.invalidate(); }
}
