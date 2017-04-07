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
  \file    plugin_mark_static_objects.h
  \brief   C++ Interface: plugin_mark_static_objects
  \author  Evgeny Sergeev, 2017

  Plugin that provides an opportunity to mark static objects on
  image received from camera and send them to all connected clients
  as a list of pairs of points over TCP.
*/
//========================================================================
#ifndef PLUGIN_MARK_STATIC_OBJECTS_H
#define PLUGIN_MARK_STATIC_OBJECTS_H

#include "static_objects/static_object_interface.h"
#include "plugin_mark_static_objects_widget.h"

#include <visionplugin.h>
#include <list>
#include <QScopedPointer>

class TcpServer;
class PluginMarkStaticObjectsWidget;

class PluginMarkStaticObjects : public VisionPlugin {
Q_OBJECT
public:
    explicit PluginMarkStaticObjects(FrameBuffer * _buffer);
    ~PluginMarkStaticObjects();
    ProcessResult process(FrameData * data, RenderOptions * options);
    virtual std::string getName();
    virtual QWidget * getControlWidget();

signals:
    void updateImage(QImage * im);
    void sendCoordinates(const std::list<StaticObjectInterface*>& objects);

public slots:
    void redrawImage();
    /* Sends coordinates to all connected clients over TCP. */
    void onSendButtonClicked();

private:
    PluginMarkStaticObjectsWidget * widget;
    QScopedPointer<TcpServer> tcpServer;
    QPainter painter;
    QPen pen;
    const int penWidth;
    QImage image;

};

#endif //PLUGIN_MARK_STATIC_OBJECTS_H
