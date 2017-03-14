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
  \brief   C++ Implementation: plugin_mark_static_objects_widget.cpp
  \author  Evgeny Sergeev, 2017
*/
//========================================================================

#include "plugin_mark_static_objects_widget.h"

PluginMarkStaticObjectsWidget::PluginMarkStaticObjectsWidget(
        PluginMarkStaticObjects * plugin, QWidget * parent, Qt::WindowFlags f) :
        QWidget(parent, f), imagePtr(), isImageRequested(false) {
    layoutMain = new QVBoxLayout(this);
    requestImageButton = new QPushButton(QString("Get image"), this);
    undoButton = new QPushButton(QString("Undo"), this);
    clearButton = new QPushButton(QString("Clear all"), this);
    sendButton = new QPushButton(QString("Send"), this);
    imageLabel = new QLabel(this);
    layoutMain->addWidget(imageLabel);
    layoutMain->addWidget(requestImageButton);
    layoutMain->addWidget(undoButton);
    layoutMain->addWidget(clearButton);
    layoutMain->addWidget(sendButton);
    this->setLayout(layoutMain);

    isFirstPointMarked = false;

    imageLabel->installEventFilter(this);
    firstPoint = QPoint();

    connect(requestImageButton,
            SIGNAL(clicked()),
            this,
            SLOT(getImageButtonClicked()));

    connect(undoButton,
            SIGNAL(clicked()),
            this,
            SLOT(undoButtonClicked()));

    connect(clearButton,
            SIGNAL(clicked()),
            this,
            SLOT(clearButtonClicked()));

    connect(sendButton,
            SIGNAL(clicked()),
            plugin,
            SLOT(onSendButtonClicked()),
            Qt::QueuedConnection);

    connect(this,
            SIGNAL(objectsChanged()),
            plugin,
            SLOT(redrawImage()),
            Qt::QueuedConnection);
}

void PluginMarkStaticObjectsWidget::update(QImage * im) {
    imageLabel->setPixmap(QPixmap::fromImage(*im));
    imagePtr.reset(im);
}

void PluginMarkStaticObjectsWidget::getImageButtonClicked() {
    isImageRequested = true;
}

void PluginMarkStaticObjectsWidget::undoButtonClicked() {
    if (!objects.empty()) {
        objects.pop_back();
        emit objectsChanged();
    }
}

void PluginMarkStaticObjectsWidget::clearButtonClicked() {
    if (!objects.empty()) {
        objects.clear();
        emit objectsChanged();
    }
}

void PluginMarkStaticObjectsWidget::imageClicked(const QMouseEvent *const event) {
    if (isFirstPointMarked) {
        objects.push_back(QPair<QPoint, QPoint>(firstPoint, event->pos()));
        isFirstPointMarked = false;
        emit objectsChanged();
    } else {
        firstPoint = event->pos();
        isFirstPointMarked = true;
    }
}

bool PluginMarkStaticObjectsWidget::eventFilter(QObject* watched, QEvent* event) {
    if (watched != imageLabel || event->type() != QEvent::MouseButtonPress)
        return false;
    const QMouseEvent* const mouseEvent = static_cast<const QMouseEvent*>(event);
    imageClicked(mouseEvent);
    return false;
}

const vector< QPair<QPoint, QPoint> >& PluginMarkStaticObjectsWidget::getObjects() const {
    return objects;
}
