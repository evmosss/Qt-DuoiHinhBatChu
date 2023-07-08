
#ifndef USER_H
#define USER_H


#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QJsonObject>
#include "database.h"
#include <QTcpSocket>
#include "protocolSocket.h"

class User
{
public:
    QJsonObject addActiveUser(int userId, QTcpSocket * socket, QList<QTcpSocket *>* activeUsers);
    void removeActiveUser(int userId, QTcpSocket * socket, QList<QTcpSocket *>* activeUsers);

    static QJsonObject getUserfromUserId(int userId);
    static void updateUserAfterAGame(int winnerId, int loserId, bool isLeave);
    int getUserFromSessionId(QString *sessionId, QList<QTcpSocket *> activeUsers, QTcpSocket *socket);
    QString getSessionIdByUser(int userId);

    User();
    QSqlDatabase database;
};

#endif // USER_H
