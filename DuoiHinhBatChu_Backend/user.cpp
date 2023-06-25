
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

QJsonObject User::addActiveUser(int userId, QTcpSocket *socket, QList<QTcpSocket *> *activeUsers)
{
    qInfo() << "Login from user: " << userId;
    activeUsers->append(socket);
    QJsonObject response;
    response["message"] = "Add active user successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = QJsonValue::Null;
    response["type"] = SocketType::FINISH_SAVE_ACTIVE_USER;
    return response;
}

void User::removeActiveUser(int userId, QTcpSocket *socket, QList<QTcpSocket *> *activeUsers)
{
    qInfo() << "Logout from user: " << userId;
    if (activeUsers->contains(socket)) {
        activeUsers->removeOne(socket);
    }
}

QJsonObject User::getUserfromUserId(int userId)
{
    QJsonObject response;
    QSqlDatabase database = Database::getInstance().getDatabase();
    QSqlQuery query(database);

    query.prepare("SELECT id, username, point FROM users where id=:id");
    query.bindValue(":id", userId);

    if (!query.exec()) {
        return QJsonObject{};
    }

    QJsonObject res;

    if (query.next()) {
        res["id"] = query.value(0).toInt();
        res["username"] = query.value(1).toString();
        res["point"] = query.value(2).toInt();
    }

    return res;
}

void User::updateUserAfterAGame(int winnerId, int loserId, bool isLeave)
{
    QJsonObject response;
    QSqlDatabase database = Database::getInstance().getDatabase();
    QSqlQuery query(database);

    //    Winner get three points
    query.prepare("UPDATE users SET point = point + 3 WHERE id=:id");
    query.bindValue(":id", winnerId);
    if (!query.exec()) {
        qInfo() << query.lastError().text();
    };

    query.clear();
    //    Quitter will be punished one point
    if (isLeave) {
        query.prepare("UPDATE users SET point = point - 1 WHERE id=:id");
        query.bindValue(":id", loserId);
    }
    else {
        query.prepare("UPDATE users SET point = point + 0 WHERE id=:id");
        query.bindValue(":id", loserId);
    }
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

