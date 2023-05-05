
#ifndef USER_H
#define USER_H


#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include "database.h"

class User
{
public:
    static void updateUserAfterAGame(int winnerId, int loserId);
    int getUserFromSessionId(QString *sessionId);
    QString getSessionIdByUser(int userId);
    User();
    QSqlDatabase database;
};

#endif // USER_H
