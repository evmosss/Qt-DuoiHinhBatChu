
#ifndef AUTH_H
#define AUTH_H

#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QObject>
#include <QString>
#include <QDebug>
#include <QCryptographicHash>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QUuid>
#include <QNetworkCookie>

class Auth
{
public:
    Auth();
    // For writing api
    QHttpServerResponse login(QString *username, QString *password);
    QHttpServerResponse registerAccount(QString *username, QString *password);

    // For simple services
    bool checkPassword(const QString &password, const QString &hashedPassword);
    QString hashPassword(const QString &password);
};

#endif // AUTH_H
