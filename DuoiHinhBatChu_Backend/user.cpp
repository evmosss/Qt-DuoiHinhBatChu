
#include "user.h"
#include "QDateTime"
#include "QHttpServerResponse"
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QVariantList>
#include <QVector>

#define _DATABASE_NAME "SP2"

int User::getUserFromSessionId(QString *sessionId)
{
    QSqlDatabase database = QSqlDatabase::database(_DATABASE_NAME);
    QSqlQuery query(database);

    query.prepare("SELECT * FROM user_sessions WHERE session_id = :sessionId");
    query.bindValue(":sessionId", *sessionId);
    query.exec();

    if (query.next()) {
        QString validDate = query.value("valid_date").toString();
        QDateTime date = QDateTime::fromString(validDate);
        QDateTime currentDate = QDateTime::currentDateTime();

        // Check for session
        if (currentDate > date) {
            return 0;
        }

        return query.value("user_id").toInt();
    }

    return 0;
}

User::User()
{

}

