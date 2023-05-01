
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
QJsonObject Room::createRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId)
{
    QJsonObject response;

    if (userToRoomId->contains(userId)) {
        response["message"] = "User has create a room before";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::BadRequest);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::CREATE_ROOM;
        return response;
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
    roomDataObject["roomId"] = *roomId;

    roomData->insert(*roomId, roomDataObject);
    userToRoomId->insert(userId, *roomId);

    response["message"] = "Create Room successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = roomDataObject;
    response["type"] = SocketType::CREATE_ROOM;
    return response;
}

QJsonObject Room::joinRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response["message"] = "Room does not exist";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::BadRequest);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::JOIN_ROOM;
        return response;
    }
    QJsonObject value = roomData->take(*roomId);
    if (value["status"].toString() == "FULL") {
        roomData->insert(*roomId, value);
        response["message"] = "Room is full";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Forbidden);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::JOIN_ROOM;
        return response;
    }
    if (value["ownerId"].toInt() == userId) {
        response["message"] = "You are owner of this room";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Forbidden);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::JOIN_ROOM;
        return response;
    }

    QJsonArray players = value["players"].toArray();
    QJsonArray points = value["points"].toArray();
    players.append(userId);
    points.append(0);
    value["players"] = players;
    value["points"] = points;
    value["status"] = "FULL";

    roomData->insert(*roomId, value);
    response["message"] = "Join room successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = value;
    response["type"] = SocketType::JOIN_ROOM;
    return response;
}

QJsonObject Room::leaveRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response["message"] = "Room does not exist";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::BadRequest);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::LEAVE_ROOM;
        return response;
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

    response["message"] = "Leave room successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = QJsonValue::Null;
    response["type"] = SocketType::LEAVE_ROOM;
    return response;
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

