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
  \file    plugin_mark_static_objects_widget.h
  \brief   C++ Interface: plugin_mark_static_objects_widget
  \author  Evgeny Sergeev, 2017
*/
//========================================================================
#ifndef PLUGIN_MARK_STATIC_OBJECTS_WIDGET_H
#define PLUGIN_MARK_STATIC_OBJECTS_WIDGET_H

#include "plugin_mark_static_objects.h"
#include "tcp_server.h"
#include "static_objects/static_object_interface.h"
#include "static_objects/point.h"
#include "static_objects/wall.h"

#include <visionplugin.h>
#include <list>
#include <QWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QMouseEvent>

class PluginMarkStaticObjects;

class PluginMarkStaticObjectsWidget : public QWidget
{
Q_OBJECT
public:
    /* Types of static object that can be marked. */
    enum StaticObjectType {
        POINT,
        WALL
    };

    PluginMarkStaticObjectsWidget(PluginMarkStaticObjects *plugin,
                                  QWidget *parent = 0, Qt::WindowFlags f = 0);

    ~PluginMarkStaticObjectsWidget();

    /* Returns array of marked static objects. */
    const std::list<StaticObjectInterface*>& getObjects() const;

    QVBoxLayout * layoutMain;
    QScopedPointer<QImage> imagePtr;
    QLabel * imageLabel;
    QPushButton * requestImageButton;
    QPushButton * undoButton;
    QPushButton * clearButton;
    QPushButton * sendButton;
    QComboBox * objectTypeComboBox;

    /* Is set to true then requestImageButton is pressed. Object of class
     * PluginMarkStaticObjects updates image if isImageRequested is true. */
    bool isImageRequested;

signals:
    /* Is emitted then content of field "objects" is changed. */
    void objectsChanged();

public slots:
    /* Sets content of field imagePtr`s to @im and updates the image on display. */
    void update(QImage * im);
    void objectTypeChanged(const QString &type);

protected:
    /* Looks for button clicks on imageLabel. */
    bool eventFilter(QObject* watched, QEvent* event);

private slots:
    void getImageButtonClicked();
    void imageClicked(const QMouseEvent *const event);
    /* Deletes last added static objects. */
    void undoButtonClicked();
    /* Deletes all static objects. */
    void clearButtonClicked();

private:
    /* Type of static object that user is going to mark. Determined by ... */
    StaticObjectType staticObjectType;
    /* Array of marked static objects */
    std::list<StaticObjectInterface*> objects;
    /* First point of a line. */
    QPoint firstPoint;
    bool isFirstPointMarked;
};

#endif //PLUGIN_MARK_STATIC_OBJECTS_WIDGET_H
