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
    staticObjectType = POINT;

    layoutMain = new QVBoxLayout(this);
    requestImageButton = new QPushButton(QString("Get image"), this);
    undoButton = new QPushButton(QString("Undo"), this);
    clearButton = new QPushButton(QString("Clear all"), this);
    sendButton = new QPushButton(QString("Send"), this);
    imageLabel = new QLabel(this);
    objectTypeComboBox = new QComboBox(this);

    objectTypeComboBox->addItem("Point");
    objectTypeComboBox->addItem("Wall");

    layoutMain->addWidget(imageLabel);
    layoutMain->addWidget(requestImageButton);
    layoutMain->addWidget(undoButton);
    layoutMain->addWidget(clearButton);
    layoutMain->addWidget(sendButton);
    layoutMain->addWidget(objectTypeComboBox);
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

    connect(objectTypeComboBox,
            SIGNAL(activated(const QString &)),
            this,
            SLOT(objectTypeChanged(const QString &)));
}

PluginMarkStaticObjectsWidget::~PluginMarkStaticObjectsWidget() {
    for(std::list<StaticObjectInterface*>::iterator object = objects.begin();
        object != objects.end(); ++object) {

        delete *object;
    }
}

void PluginMarkStaticObjectsWidget::update(QImage * im) {
    imageLabel->setPixmap(QPixmap::fromImage(*im));
    imagePtr.reset(im);
}

void PluginMarkStaticObjectsWidget::objectTypeChanged(const QString & type) {
    if (QString::compare(type, QString("Wall")) == 0) {
        staticObjectType = WALL;
    } else if (QString::compare(type, QString("Point")) == 0) {
        staticObjectType = POINT;
    } else {
        qDebug() << "Unknown static object type: " << type.toUtf8().constData();
    }
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
    QPoint point = event->pos();

    switch(staticObjectType) {
        case POINT:
            objects.push_back(new Point(point.x(), point.y()));
            emit objectsChanged();
            break;

        case WALL:
            if (isFirstPointMarked) {
                objects.push_back(new Wall(firstPoint.x(), firstPoint.y(), point.x(), point.y()));
                isFirstPointMarked = false;
                emit objectsChanged();
            } else {
                firstPoint = event->pos();
                isFirstPointMarked = true;
            }
            break;
    }
}

bool PluginMarkStaticObjectsWidget::eventFilter(QObject* watched, QEvent* event) {
    if (watched != imageLabel || event->type() != QEvent::MouseButtonPress)
        return false;
    const QMouseEvent* const mouseEvent = static_cast<const QMouseEvent*>(event);
    imageClicked(mouseEvent);
    return false;
}

const list<StaticObjectInterface*>& PluginMarkStaticObjectsWidget::getObjects() const {
    return objects;
}
