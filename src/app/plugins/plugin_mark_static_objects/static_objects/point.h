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
  \file    point.h
  \brief   C++ Interface: point
  \author  Evgeny Sergeev, 2017

  Representation of point static object.
*/
//========================================================================

#ifndef SSL_VISION_POINT_H
#define SSL_VISION_POINT_H

#include "static_object_interface.h"

class Point : public StaticObjectInterface {
public:
    Point(int x, int y);
    virtual ~Point();
    virtual void serialize(StaticObjectsProtobuf::StaticObjects &staticObjects) const;
    virtual void draw(QPainter &painter) const;
private:
    const int mXCoord;
    const int mYCoord;
};

#endif //SSL_VISION_POINT_H
