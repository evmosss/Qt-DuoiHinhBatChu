
#include "room.h"
#include "QHttpServerResponse"
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QVariantList>
#include <QVector>

#define _DATABASE_NAME "SP2"

bool validateSession(QString sessionId) {
    QSqlDatabase database = QSqlDatabase::database(_DATABASE_NAME);
    QSqlQuery query(database);

    query.prepare("SELECT * FROM user_sessions WHERE session_id = :sessionId");
    query.bindValue(":sessionId", sessionId);
    query.exec();

    if (query.next()) {
        // Không cần check xem sessionId có hết hạn chưa vì đã check ở controller rồi
        return true;
    }
    return false;
}

// Apis that will be change to Tcp logic in the future
QHttpServerResponse Room::createRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId)
{
    if (userToRoomId->contains(userId)) {
        return QHttpServerResponse(QJsonObject{ {"message", "User has create a room before" } }, QHttpServerResponder::StatusCode::Ok);
    }

    QJsonObject roomDataObject;
    roomDataObject["players"] = QJsonArray::fromVariantList(QVariantList() << userId);
    roomDataObject["points"] = QJsonArray::fromVariantList(QVariantList() << 0);
    roomDataObject["ownerId"] = userId;
    roomDataObject["status"] = "PENDING";
    roomDataObject["questionIndex"] = 0;
    roomDataObject["questionUrl"] = "";
    roomDataObject["questionAnswer"] = "";
    roomDataObject["questionLevel"] = 0;
    roomDataObject["maxQuestions"] = 10;

    roomData->insert(*roomId, roomDataObject);
    userToRoomId->insert(userId, *roomId);

    return QHttpServerResponse(QJsonObject{ {"message", "Create Room successfully"} }, QHttpServerResponder::StatusCode::Ok);
}

QHttpServerResponse Room::joinRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    if (!roomData->contains(*roomId)) {
        return QHttpServerResponse(QJsonObject{ {"message", "Room does not exist"} }, QHttpServerResponder::StatusCode::BadRequest);
    }
    QJsonObject value = roomData->take(*roomId);
    if (value["status"].toString() == "FULL") {
        roomData->insert(*roomId, value);
        return QHttpServerResponse(QJsonObject{ {"message", "Room is full"} }, QHttpServerResponder::StatusCode::BadRequest);
    }
    if (value["ownerId"].toInt() == userId) {
        return QHttpServerResponse(QJsonObject{ {"message", "You are owner of this room"} }, QHttpServerResponder::StatusCode::BadRequest);
    }

    QJsonArray players = value["players"].toArray();
    QJsonArray points = value["points"].toArray();
    players.append(userId);
    points.append(0);
    value["players"] = players;
    value["points"] = points;
    value["status"] = "FULL";

    roomData->insert(*roomId, value);
    return QHttpServerResponse(QJsonObject{ {"message", "Join room successfully"} }, QHttpServerResponder::StatusCode::Ok);
}

QHttpServerResponse Room::leaveRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    if (!roomData->contains(*roomId)) {
        return QHttpServerResponse(QJsonObject{ {"message", "Room does not exist"} }, QHttpServerResponder::StatusCode::BadRequest);
    }

    if (userToRoomId->contains(userId)) {
        userToRoomId->remove(userId);
        roomData->remove(*roomId);
    }
    else {
        QJsonObject json = roomData->take(*roomId);
        QJsonArray players = json.value("players").toArray();
        QJsonArray points = json.value("points").toArray();

        points[0] = 0;
        points.pop_back();
        players.pop_back();
        json["players"] = players;
        json["points"] = points;
        json["questionIndex"] = 0;
        json["questionId"] = 0;
        json["status"] = "PENDING";

        roomData->insert(*roomId, json);
    }
    return QHttpServerResponse(QJsonObject{ {"message", "Leave room successfully"} }, QHttpServerResponder::StatusCode::Ok);
}

QHttpServerResponse Room::startGame(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    if (!roomData->contains(*roomId)) {
        return QHttpServerResponse(QJsonObject{ {"message", "Room does not exist"} }, QHttpServerResponder::StatusCode::BadRequest);
    }

    QJsonObject json = roomData->take(*roomId);

    if (json["ownerId"].toInt() != userId) {
        return QHttpServerResponse(QJsonObject{ {"message", "You are not owner of this room"} }, QHttpServerResponder::StatusCode::Forbidden);
    }

    if (json["status"] != "FULL") {
        return QHttpServerResponse(QJsonObject{ {"message", "Room does not have enough participants"} }, QHttpServerResponder::StatusCode::Forbidden);
    }

    json["status"] = "STARTING";
    roomData->insert(*roomId, json);
    return QHttpServerResponse(QJsonObject{ {"message", "Start Game successfully"} }, QHttpServerResponder::StatusCode::Ok);
}

QHttpServerResponse Room::getAllRoom(QMap<QString, QJsonObject> roomDataMap)
{

    QJsonObject resultObject;
    for (auto itr = roomDataMap.begin(); itr != roomDataMap.end(); ++itr) {
        resultObject[itr.key()] = itr.value();
    }

    return QHttpServerResponse(QJsonObject{ {"message", "Get All Room successfully"}, {"data", resultObject} }, QHttpServerResponder::StatusCode::Ok);
}

Room::Room()
{

}

