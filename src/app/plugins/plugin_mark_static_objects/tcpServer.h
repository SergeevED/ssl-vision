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
  \file    tcpServer.h
  \brief   C++ Interface: tcpServer.h
  \author  Evgeny Sergeev, 2017
*/
//========================================================================
#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <QtNetwork>
#include <QWidget>
#include <QMutex>
#include <QByteArray>
#include <vector>
#include "messages_robocup_ssl_static_objects.pb.h"

class TcpServer : public QObject {
Q_OBJECT

public:
    explicit TcpServer(QWidget *parent = 0);
    ~TcpServer();

public slots:
    /* Sends message to all connected clients. */
    void sendMessageToAll(const std::vector< QPair<QPoint, QPoint> >& objects);

private slots:
    void onNewConnection();

private:
    /* Next port to be used. Server reads the value and increments it atomically. */
    static quint16 nextPort;
    static QMutex mtx;
    /* Port on which server runs. */
    quint16 uniquePort;
    QScopedPointer<QTcpServer> tcpServerPtr;
    QMap<int, QTcpSocket *> clients;
    bool isServerRun;
};

#endif //TCP_SERVER_H
