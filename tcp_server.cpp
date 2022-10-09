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
{connected_devices_.insert(descriptor, new ClientThread(this, descriptor));}

ClientThread::ClientThread(QObject *parent, qintptr descriptor)
    : QThread(parent)
{
    if(descriptor != 0) {
        socket_ = new QTcpSocket;
        if(socket_->setSocketDescriptor(descriptor)) {
            this->descriptor_ = descriptor;
            this->ip_ = socket_->peerName();
            this->start();
            QObject::connect(socket_, SIGNAL(stateChanged(QAbstractSocket::SocketState)),
                             this, SLOT(onStateChanged(QAbstractSocket::SocketState)));
            QObject::connect(socket_, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
            QObject::connect(this, SIGNAL(finished()), SLOT(deleteLater()));
            qDebug() << "server -> new socket connected";
        } else {
            qDebug() << "server -> can't set descriptor for new socket";
        }
    }
}

void ClientThread::onStateChanged(QAbstractSocket::SocketState state)
{
    if(state == QAbstractSocket::UnconnectedState)
        exit();
}

void ClientThread::onReadyRead()
{
    
}
