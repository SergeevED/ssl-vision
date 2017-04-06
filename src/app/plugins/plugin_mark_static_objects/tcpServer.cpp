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

quint16 TcpServer::nextPort(10007);
QMutex TcpServer::mtx;

TcpServer::TcpServer(QWidget * parent) : QObject(parent), tcpServerPtr(new QTcpServer(this)) {
    isServerRun = false;
    clients = QMap<int, QTcpSocket *>();
    connect(tcpServerPtr.data(), SIGNAL(newConnection()), this, SLOT(onNewConnection()));

    mtx.lock();
    uniquePort = nextPort++;
    mtx.unlock();

    if (!tcpServerPtr->listen(QHostAddress::Any, uniquePort)) {
        qDebug() <<  QObject::tr("Unable to start the server: %1.").arg(tcpServerPtr->errorString());
    } else {
        isServerRun = true;
        qDebug() <<  QObject::tr("TCPSocket listen on port %1.").arg(uniquePort);
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

void TcpServer::sendMessageToAll(const std::vector< QPair<QPoint, QPoint> >& objects) {
    if (!isServerRun) {
        return;
    }

    StaticObjects staticObjects;

    for(std::vector<QPair<QPoint, QPoint> >::const_iterator point = objects.begin();
        point != objects.end(); ++point) {

        Point *first = new Point();
        first->set_x(point->first.x());
        first->set_y(point->first.y());

        Point *second = new Point();
        second->set_x(point->second.x());
        second->set_y(point->second.y());

        Wall *wall = staticObjects.add_walls();
        wall->set_allocated_beginning(first);
        wall->set_allocated_end(second);
    }

    QScopedPointer<std::string> staticObjectsString(new std::string());
    staticObjects.SerializeToString(&*staticObjectsString);

    QMap<int, QTcpSocket *>::const_iterator i = clients.constBegin();
    while (i != clients.constEnd()) {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_0);
        out << static_cast<quint16>(0);
        out << staticObjectsString->data();
        out.device()->seek(0);
        out << static_cast<quint16>(block.size() - sizeof(quint16));
        i.value()->write(block);
        ++i;
    }
}
