
#include "room.h"
#include "QHttpServerResponse"
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QStringList>
#include <QVariantList>
#include <QVector>
#include "question.h"

#define _DATABASE_NAME "SP2"

bool validateSession(QString sessionId) {
    QSqlDatabase database = Database::getInstance().getDatabase();
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
int Room::findUserInArray(int userId, QJsonArray players)
{
    int userIndex = -1;
    for (int i = 0; i < players.size(); i++) {
        if (players[i].toInt() == userId) {
            userIndex = i;
            break;
        }
    }
    return userIndex;
}

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
    roomDataObject["questionData"] = QJsonValue::Null;
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

    QJsonObject json = roomData->take(*roomId);
    QJsonArray players = json.value("players").toArray();
    QJsonArray points = json.value("points").toArray();
    if (userToRoomId->contains(userId)) {
        Room::handleUpdateFinishGame(players.at(1).toInt(), players.at(0).toInt());

        userToRoomId->remove(userId);
        roomData->remove(*roomId);
        response["data"] = QJsonValue::Null;
    }
    else {
        // Avoid clicking two times
        if (players.size() == 1) {
            response["message"] = "You already leave room!";
            response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Forbidden);
            response["data"] = QJsonValue::Null;
            response["type"] = SocketType::LEAVE_ROOM;
            return response;
        }

        Room::handleUpdateFinishGame(players.at(0).toInt(), players.at(1).toInt());

        int leavePlayerId = players.at(1).toInt();
        points[0] = 0;
        points.pop_back();
        players.pop_back();
        json["players"] = players;
        json["points"] = points;
        json["questionIndex"] = 0;
        json["questionId"] = 0;
        json["status"] = "PENDING";

        QJsonObject resData;
        resData["leavePlayerId"] = leavePlayerId;
        response["data"] = resData;
    }

    roomData->insert(*roomId, json);

    response["message"] = "Leave room successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["type"] = SocketType::LEAVE_ROOM;
    return response;
}

QJsonObject Room::sendAnswer(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId, QString content)
{
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response["message"] = "Room does not exist";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::BadRequest);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::SEND_ANSWER;
        return response;
    }

    QJsonObject returnData;
    returnData["isTrue"] = false;
    returnData["content"] = "User " + QString::number(userId) + ": " + content + ".";

    QJsonObject roomDetail = roomData->take(*roomId);
    QJsonArray players = roomDetail.value("players").toArray();
    QJsonArray points = roomDetail.value("points").toArray();

    int userIndex = findUserInArray(userId, players);
    int playerPoints = 0;

    int questionIndex = roomDetail.value("questionIndex").toInt();
    QJsonObject questionData = roomDetail.value("questionData").toObject();
    if (questionIndex > 0) {
        QString answer = questionData.value("answer").toString();
        if (answer.toLower() == content.toLower()) {
            returnData["isTrue"] = true;

            playerPoints = points[userIndex].toInt();
            playerPoints++;
            points[userIndex] = playerPoints;
        }
    }

    roomDetail["points"] = points;
    roomData->insert(*roomId, roomDetail);

    returnData["roomDetail"] = roomDetail;

    response["message"] = "Send answer successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = returnData;
    response["type"] = SocketType::SEND_ANSWER;
    return response;
}

QJsonObject Room::startRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response["message"] = "Room does not exist";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::BadRequest);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::START_ROOM;
        return response;
    }

    QJsonObject questionData = Question::getRandomQuestion();
    QJsonObject value = roomData->take(*roomId);

    value["questionData"] = questionData;
    value["questionIndex"] = 1; // Starting with first question

    // Assign all points to zero
    QJsonArray pointsArray;
    pointsArray.append(0);
    pointsArray.append(0);
    value["points"] = pointsArray;

    roomData->insert(*roomId, value);

    response["message"] = "Start Room Successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = value;
    response["type"] = SocketType::START_ROOM;

    return response;
}

QJsonObject Room::nextQuestion(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response["message"] = "Room does not exist";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::BadRequest);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::NEXT_QUESTION;
        return response;
    }

    QJsonObject value = roomData->take(*roomId);
    int ownerId = value.value("ownerId").toInt();

    if (userId != ownerId) {
        response["message"] = "Only owner can get next question";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Forbidden);
        response["data"] = QJsonValue::Null;
        response["type"] = SocketType::NEXT_QUESTION;
    }

    int qIndex = value.value("questionIndex").toInt();
    int maxQuestion = value.value("maxQuestions").toInt();

    if (qIndex < maxQuestion) {
        QJsonObject questionData = Question::getRandomQuestion();
        value["questionData"] = questionData;
        qIndex++;

        value["questionData"] = questionData;
        value["questionIndex"] = qIndex;

        roomData->insert(*roomId, value);

        response["message"] = "Get next question to room Successfully";
        response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
        response["data"] = value;
        response["type"] = SocketType::NEXT_QUESTION;

        return response;
    }
    else {
        roomData->insert(*roomId, value);
        return Room::finishGame(userId, roomData, roomId, userToRoomId);
    }

}

QJsonObject Room::finishGame(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    QJsonObject response;
    QJsonObject responseData;
    QJsonObject roomDetail = roomData->take(*roomId);

    QJsonArray players = roomDetail.value("players").toArray();
    QJsonArray points = roomDetail.value("points").toArray();

    if (points.at(0).toInt() < points.at(1).toInt()) {
        responseData["winnerId"] = players.at(1).toInt();

        Room::handleUpdateFinishGame(players.at(1).toInt(), players.at(0).toInt());
    }
    else if (points.at(0).toInt() > points.at(1).toInt()) {
        responseData["winnerId"] = players.at(0).toInt();

        Room::handleUpdateFinishGame(players.at(0).toInt(), players.at(1).toInt());
    }
    else {
        responseData["winnerId"] = 0;
    }

    roomData->insert(*roomId, roomDetail);

    response["message"] = "Finish Room Successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = responseData;
    response["type"] = SocketType::FINISH_ROOM;
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

void Room::handleUpdateFinishGame(int winnerId, int loserId)
{
    History::createHistory(winnerId, loserId);
    User::updateUserAfterAGame(winnerId, loserId);
}

