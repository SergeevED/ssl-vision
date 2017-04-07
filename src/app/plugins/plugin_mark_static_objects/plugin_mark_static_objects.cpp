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
  \file    plugin_mark_static_objects.cpp
  \brief   C++ Implementation: plugin_mark_static_objects.cpp
  \author  Evgeny Sergeev, 2017
*/
//========================================================================

#include "plugin_mark_static_objects.h"

PluginMarkStaticObjects::PluginMarkStaticObjects(FrameBuffer * _buffer) :
        VisionPlugin(_buffer), tcpServer(new TcpServer()), painter(), penWidth(4) {
    widget = new PluginMarkStaticObjectsWidget(this);
    pen = QPen();
    pen.setColor(Qt::cyan);
    pen.setWidth(penWidth);
    image = QImage();

    connect(this,
            SIGNAL(updateImage(QImage *)),
            widget,
            SLOT(update(QImage *)),
            Qt::QueuedConnection);

    connect(this,
            SIGNAL(sendCoordinates(const std::list<StaticObjectInterface*> &)),
            &*tcpServer,
            SLOT(sendMessageToAll(const std::list<StaticObjectInterface*> &)));
}

PluginMarkStaticObjects::~PluginMarkStaticObjects() {
    delete widget;
}

ProcessResult PluginMarkStaticObjects::process(FrameData *data, RenderOptions *options) {
    if (widget->isImageRequested) {
        widget->isImageRequested = false;
        // deep copy
        image = QImage(data->video.getData(),
                                 data->video.getWidth(),
                                 data->video.getHeight(),
                                 QImage::Format_RGB888).copy();
        redrawImage();
    }

    return ProcessingOk;
}

void PluginMarkStaticObjects::redrawImage() {
    // deep copy
    QImage * im = new QImage(image.copy());
    painter.begin(im);
    painter.setPen(pen);
    const std::list<StaticObjectInterface*>& objects = widget->getObjects();
    for(std::list<StaticObjectInterface*>::const_iterator object = objects.begin();
        object != objects.end(); ++object) {

        (*object)->draw(painter);
    }
    painter.end();

    emit updateImage(im);
}

void PluginMarkStaticObjects::onSendButtonClicked() {
    QString message;
    const std::list<StaticObjectInterface*>& objects = widget->getObjects();
    if (objects.empty()) {
        return;
    }

    emit sendCoordinates(objects);
}

std::string PluginMarkStaticObjects::getName() {
    return "Mark static objects";
}

QWidget *PluginMarkStaticObjects::getControlWidget() {
    return widget;
}
