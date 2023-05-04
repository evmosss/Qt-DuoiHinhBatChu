#ifndef SOCKET_H
#define SOCKET_H

#include <QObject>
#include <QTcpSocket>

class Socket : public QObject
{
    Q_OBJECT
public:
    static Socket& getInstance();

    void connectToHost(const QString& host, quint16 port);
    void disconnectFromHost();
    void write(const QByteArray& data);
    QTcpSocket* socket() const { return m_socket; }

private:
    explicit Socket(QObject *parent = nullptr);
    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    QTcpSocket* m_socket;
};

#endif // SOCKET_H
