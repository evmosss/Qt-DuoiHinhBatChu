#include "socket.h"

Socket& Socket::getInstance()
{
    static Socket instance;
    return instance;
}

void Socket::connectToHost(const QString& host, quint16 port)
{
    m_socket->connectToHost(host, port);
}

void Socket::disconnectFromHost()
{
    m_socket->disconnectFromHost();
}

void Socket::write(const QByteArray& data)
{
    m_socket->write(data);
}

Socket::Socket(QObject *parent)
    : QObject(parent)
{
    m_socket = new QTcpSocket(this);
}
