#include "point.h"

Point::Point(int x, int y) : StaticObjectInterface(), mXCoord(x), mYCoord(y) {
}

Point::~Point() {
}

void Point::serialize(StaticObjectsProtobuf::StaticObjects &staticObjects) const {
    StaticObjectsProtobuf::Point *point = staticObjects.add_points();
    point->set_x(mXCoord);
    point->set_y(mYCoord);
}

void Point::draw(QPainter &painter) const {
    painter.drawPoint(mXCoord, mYCoord);
}
