
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

void User::updateUserAfterAGame(int winnerId, int loserId)
{
    QJsonObject response;
    QSqlDatabase database = Database::getInstance().getDatabase();
    QSqlQuery query(database);

    query.prepare("UPDATE users SET winner_times = winner_times + 1 WHERE id=:id");
    query.bindValue(":id", winnerId);
    if (!query.exec()) {
        qInfo() << query.lastError().text();
    };

    query.clear();
    query.prepare("UPDATE users SET lose_times = lose_times + 1 WHERE id=:id");
    query.bindValue(":id", loserId);
    if (!query.exec()) {
        qInfo() << query.lastError().text();
    };

}

int User::getUserFromSessionId(QString *sessionId)
{
    database = Database::getInstance().getDatabase();
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

QString User::getSessionIdByUser(int userId)
{

}

User::User()
{

}

