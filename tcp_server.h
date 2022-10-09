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
    
protected:
    void incomingConnection(qintptr descriptor);
    
private:
    QMap<qintptr, ClientThread*> connected_devices_;
};

class ClientThread : public QThread
{
    Q_OBJECT
public:
    ClientThread(QObject *parent, qintptr descriptor);
    
public slots:
    void onReadyRead();
    void onStateChanged(QAbstractSocket::SocketState);
    
private:
    QString ip_;
    qintptr descriptor_;
    QTcpSocket *socket_;
};

#endif // MYTCPSERVER_H
