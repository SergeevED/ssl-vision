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
  \file    staticObject.h
  \brief   C++ Interface: staticObject
  \author  Evgeny Sergeev, 2017

  Interface for all static objects to be derived.
*/
//========================================================================
#ifndef SSL_VISION_STATIC_OBJECT_INTERFACE_H
#define SSL_VISION_STATIC_OBJECT_INTERFACE_H

#include "messages_robocup_ssl_static_objects.pb.h"

#include <QPainter>

class StaticObjectInterface {
public:
    virtual ~StaticObjectInterface() { };
    // Serialize object`s content and add to protobuf message.
    virtual void serialize(StaticObjectsProtobuf::StaticObjects &staticObjects) const = 0;
    // Draw object`s content on image the painter is associated with.
    virtual void draw(QPainter &painter) const = 0;
};

#endif //SSL_VISION_STATIC_OBJECT_INTERFACE_H
