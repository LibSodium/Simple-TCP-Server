#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QThread>
#include <QDateTime>
#include <QTcpSocket>
#include <QTcpServer>

class ClientThread;

class MyTcpServer : public QTcpServer
{
    Q_OBJECT
public:
    MyTcpServer(qint16 port);
    QMap<qintptr, ClientThread*> connected_devices;
    
protected:
    void incomingConnection(qintptr descriptor);

};

class ClientThread : public QThread
{
    Q_OBJECT
public:
    ClientThread(qintptr descriptor, MyTcpServer *server);
    
public slots:
    void onReadyRead();
    void onStateChanged(QAbstractSocket::SocketState);
    
private:
    QString     ip_;
    qintptr     descriptor_;
    QTcpSocket  *socket_;
    MyTcpServer *server_;
};

#endif // MYTCPSERVER_H
