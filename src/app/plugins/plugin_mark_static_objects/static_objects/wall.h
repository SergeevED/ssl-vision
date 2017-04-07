//========================================================================
//  This software is free: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License Version 3,
//  as published by the Free Software Foundation.
//
//  This software is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  Version 3 in the file COPYING that came with this distribution.
//  If not, see <http://www.gnu.org/licenses/>.
//========================================================================
/*!
  \file    wall.h
  \brief   C++ Interface: wall
  \author  Evgeny Sergeev, 2017

  Representation of wall static object.
*/
//========================================================================
#ifndef SSL_VISION_WALL_H
#define SSL_VISION_WALL_H

#include "static_object_interface.h"

class Wall : public StaticObjectInterface {
public:
    Wall(int x1, int y1, int x2, int y2);
    virtual ~Wall();
    virtual void serialize(StaticObjectsProtobuf::StaticObjects &staticObjects) const;
    virtual void draw(QPainter &painter) const;
private:
    const int mX1Coord;
    const int mY1Coord;
    const int mX2Coord;
    const int mY2Coord;
};

#endif //SSL_VISION_WALL_H
