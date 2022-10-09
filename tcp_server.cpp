#include "tcp_server.h"

MyTcpServer::MyTcpServer(qint16 port) : QTcpServer(NULL)
{
    if(this->listen(QHostAddress::Any, port)) {
        qDebug() << "server -> started";
    } else {
        qDebug() << "server -> not started";
    }
}

void MyTcpServer::incomingConnection(qintptr descriptor)
{connected_devices.insert(descriptor, new ClientThread(descriptor, this));}

ClientThread::ClientThread(qintptr descriptor, MyTcpServer *server)
    : QThread(server)
{
    if(descriptor != 0) {
        socket_ = new QTcpSocket;
        if(socket_->setSocketDescriptor(descriptor)) {
            this->descriptor_ = descriptor;
            this->ip_ = socket_->peerName();
            this->server_ = server;
            QObject::connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                             this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
            QObject::connect(socket_, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            QObject::connect(this, SIGNAL(finished()), SLOT(deleteLater()));
            this->start();
            qDebug() << "server -> new socket connected";
        } else {
            qDebug() << "server -> can't set descriptor for new socket";
        }
    }
}

void ClientThread::onStateChanged(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState) {
        server_->connected_devices.remove(descriptor_); exit();
    }
}

void ClientThread::onReadyRead()
{
    
}
