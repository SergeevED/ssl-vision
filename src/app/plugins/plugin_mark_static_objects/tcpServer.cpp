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
  \brief   C++ Implementation: tcpServer.cpp
  \author  Evgeny Sergeev, 2017
*/
//========================================================================

#include "tcpServer.h"

TcpServer::TcpServer(QWidget * parent) : QObject(parent), port(10007), tcpServerPtr(new QTcpServer(this)) {
    isServerRun = false;
    clients = QMap<int, QTcpSocket *>();
    connect(tcpServerPtr.data(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));
    if (!tcpServerPtr->listen(QHostAddress::Any, port)) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServerPtr->errorString());
    } else {
        isServerRun = true;
        qDebug() <<  QObject::tr("TCPSocket listen on port %1.").arg(port);
    }
}

TcpServer::~TcpServer() {
    QMap<int, QTcpSocket *>::iterator i = clients.begin();
    while (i != clients.constEnd()) {
        delete i.value();
    }
}

void TcpServer::onNewConnection() {
    if (!isServerRun) {
        return;
    }
    QTcpSocket* clientSocket = tcpServerPtr->nextPendingConnection();
    int socketDescriptor = clientSocket->socketDescriptor();
    clients[socketDescriptor] = clientSocket;
}

void TcpServer::sendMessageToAll(QByteArray *data) {
    if (!isServerRun) {
        return;
    }
    QMap<int, QTcpSocket *>::const_iterator i = clients.constBegin();
    while (i != clients.constEnd()) {
        i.value()->write(*data);
        ++i;
    }
    delete data;
}
