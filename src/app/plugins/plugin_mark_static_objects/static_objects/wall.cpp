#include "wall.h"

Wall::Wall(int x1, int y1, int x2, int y2)
        : StaticObjectInterface(), mX1Coord(x1), mY1Coord(y1), mX2Coord(x2), mY2Coord(y2) {
}

Wall::~Wall() {
}

void Wall::serialize(StaticObjectsProtobuf::StaticObjects &staticObjects) const {
    StaticObjectsProtobuf::Point *first = new StaticObjectsProtobuf::Point();
    first->set_x(mX1Coord);
    first->set_y(mY1Coord);

    StaticObjectsProtobuf::Point *second = new StaticObjectsProtobuf::Point();
    second->set_x(mX2Coord);
    second->set_y(mY2Coord);

    StaticObjectsProtobuf::Wall *wall = staticObjects.add_walls();
    wall->set_allocated_beginning(first);
    wall->set_allocated_end(second);
}

void Wall::draw(QPainter &painter) const {
    painter.drawLine(mX1Coord, mY1Coord, mX2Coord, mY2Coord);
}
