
#ifndef USER_H
#define USER_H


#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

class User
{
public:
    int getUserFromSessionId(QString *sessionId);
    User();
};

#endif // USER_H
