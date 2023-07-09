
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
#include "util.h"

#define _DATABASE_NAME "SP2"
extern int DEFAULT_MAX_QUESTIONS;

// WE HAVE FOUR STATES FOR ROOM: PENDING, FULL, STARTED, FINISHED

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
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::BadRequest, "User has create a room before", SocketType::CREATE_ROOM);
        return response;
    }

    QJsonObject ownerData = User::getUserfromUserId(userId);
    int ownerPoint = ownerData.value("point").toInt();
    qInfo() << "Owner point" << ownerPoint;

    QJsonObject roomDataObject;
    roomDataObject["players"] = QJsonArray::fromVariantList(QVariantList() << userId);
    roomDataObject["points"] = QJsonArray::fromVariantList(QVariantList() << 0);
    roomDataObject["ownerId"] = userId;
    roomDataObject["status"] = "PENDING";
    roomDataObject["questionIndex"] = 0;
    roomDataObject["questionData"] = QJsonValue::Null;
    roomDataObject["maxQuestions"] = DEFAULT_MAX_QUESTIONS;
    roomDataObject["roomId"] = *roomId;
    roomDataObject["difficulty"] = static_cast<int>(getRoomDifficultyFromPoint(ownerPoint));

    qInfo() << "Owner class" << static_cast<int>(getRoomDifficultyFromPoint(ownerPoint));

    roomData->insert(*roomId, roomDataObject);
    userToRoomId->insert(userId, *roomId);

    qInfo() << "[+] Create Room: " << *roomData;
    qInfo() << "[+] Create Room: " << *userToRoomId;

    response = createSocketResponse(roomDataObject, QHttpServerResponder::StatusCode::Ok, "Create Room successfully", SocketType::CREATE_ROOM);
    return response;
}

QJsonObject Room::joinRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    bool error = false;
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::BadRequest, "Room does not exist", SocketType::JOIN_ROOM);
        error = true;
        return response;
    }
    QJsonObject value = roomData->take(*roomId);
    if (value["status"].toString() != "PENDING") {
        roomData->insert(*roomId, value);
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::Forbidden, "Room is full", SocketType::JOIN_ROOM);
        error = true;
    }
    if (value["ownerId"].toInt() == userId) {
        roomData->insert(*roomId, value);
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::Forbidden, "You are owner of this room", SocketType::JOIN_ROOM);
        error = true;
    }

    QJsonObject joinData = User::getUserfromUserId(userId);
    int joinPoint = joinData.value("point").toInt();
    if (!canJoinRoom(static_cast<RoomDifficulty>(value["difficulty"].toInt()),joinPoint)) {
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::Forbidden, "You are not in this difficulty class", SocketType::JOIN_ROOM);
        error = true;
    }

    if (error) {
        roomData->insert(*roomId, value);
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
    response = createSocketResponse(value, QHttpServerResponder::StatusCode::Ok, "Join room successfully", SocketType::JOIN_ROOM);
    return response;
}

QJsonObject Room::leaveRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    qInfo() << "[+]" << userId;
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::BadRequest, "Room does not exist", SocketType::LEAVE_ROOM);
        return response;
    }

    QJsonObject json = roomData->take(*roomId);
    QJsonArray players = json.value("players").toArray();
    QJsonArray points = json.value("points").toArray();
    QString status = json.value("status").toString();

    if (userToRoomId->contains(userId)) {
        if (status == "STARTED") {
            Room::handleUpdateFinishGame(players.at(1).toInt(), players.at(0).toInt(), true);
        }

        userToRoomId->remove(userId);
        roomData->remove(*roomId);
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::Ok, "Leave room successfully", SocketType::LEAVE_ROOM);
    }
    else {
        if (status == "STARTED") {
            Room::handleUpdateFinishGame(players.at(0).toInt(), players.at(1).toInt(), true);
        }

        int leavePlayerId = players.at(1).toInt();
        points[0] = 0;
        points.pop_back();
        players.pop_back();
        json["players"] = players;
        json["points"] = points;
        json["status"] = "PENDING";

        QJsonObject resData;
        resData["leavePlayerId"] = leavePlayerId;
        response =  createSocketResponse(resData, QHttpServerResponder::StatusCode::Ok, "Leave room successfully", SocketType::LEAVE_ROOM);

        roomData->insert(*roomId, json);
    }


    qInfo() << "[+] Leave Room: " << *roomData;
    qInfo() << "[+] Leave Room: " << *userToRoomId;

    return response;
}

QJsonObject Room::sendAnswer(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId, QString content)
{
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::BadRequest, "Room does not exist", SocketType::SEND_ANSWER);
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

    response = createSocketResponse(returnData, QHttpServerResponder::StatusCode::Ok, "Send answer successfully", SocketType::SEND_ANSWER);
    return response;
}

QJsonObject Room::startRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::BadRequest, "Room does not exist", SocketType::START_ROOM);
        return response;
    }

    QJsonObject questionData = Question::getRandomQuestion();
    QJsonObject value = roomData->take(*roomId);
    QString status = value.value("status").toString();

    if (status != "FULL" && status != "FINISHED") {
        roomData->insert(*roomId, value);
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::BadRequest, "Room is not in correct state", SocketType::START_ROOM);
        return response;
    }

    value["questionData"] = questionData;
    value["questionIndex"] = 1; // Starting with first question
    value["status"] = "STARTED";

    // Assign all points to zero
    QJsonArray pointsArray;
    pointsArray.append(0);
    pointsArray.append(0);
    value["points"] = pointsArray;

    roomData->insert(*roomId, value);

    response = createSocketResponse(value, QHttpServerResponder::StatusCode::Ok, "Start Room Successfully", SocketType::START_ROOM);
    return response;
}

QJsonObject Room::nextQuestion(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{
    qInfo() << "[+] Caller: " << userId << "[+] Room Data:" << *roomData << "[+] Room Id:" << *roomId;
    QJsonObject response;
    if (!roomData->contains(*roomId)) {
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::BadRequest, "Start Room Successfully", SocketType::NEXT_QUESTION);
        return response;
    }

    QJsonObject value = roomData->take(*roomId);
    int ownerId = value.value("ownerId").toInt();
    QString status = value.value("status").toString();

    if (userId != ownerId) {
        roomData->insert(*roomId, value);
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::Forbidden, "Only owner can get next question", SocketType::NEXT_QUESTION);
        return response;
    }

    if (status != "STARTED") {
        roomData->insert(*roomId, value);
        response = createSocketResponse(QJsonObject {}, QHttpServerResponder::StatusCode::Forbidden, "Room has to be in starting status", SocketType::NEXT_QUESTION);
        return response;
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

        response = createSocketResponse(value, QHttpServerResponder::StatusCode::Ok, "Get next question to room Successfully", SocketType::NEXT_QUESTION);

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
    QString status = roomDetail.value("status").toString();

    if (status != "STARTED") {

    }

    if (points.at(0).toInt() < points.at(1).toInt()) {
        responseData["winnerId"] = players.at(1).toInt();

        Room::handleUpdateFinishGame(players.at(1).toInt(), players.at(0).toInt(), false);
    }
    else if (points.at(0).toInt() > points.at(1).toInt()) {
        responseData["winnerId"] = players.at(0).toInt();

        Room::handleUpdateFinishGame(players.at(0).toInt(), players.at(1).toInt(), false);
    }
    else {
        responseData["winnerId"] = 0;
    }
    roomDetail["status"] = "FINISHED";

    roomData->insert(*roomId, roomDetail);

    response["message"] = "Finish Room Successfully";
    response["code"] = static_cast<int>(QHttpServerResponder::StatusCode::Ok);
    response["data"] = responseData;
    response["type"] = SocketType::FINISH_ROOM;

    return response;
}

QJsonObject Room::getAllCurrentRoom(int userId, QMap<QString, QJsonObject> roomData)
{
    qInfo() << "[+] Send all room to user: " << userId;

    QJsonObject response;
    QJsonObject obj;
    for (auto it = roomData.begin(); it != roomData.end(); ++it) {
        obj.insert(it.key(), it.value());
    }

    response = createSocketResponse(obj, QHttpServerResponder::StatusCode::Ok, "Get All Room Successfully", SocketType::REQUEST_ALL_ROOM);
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

    json["status"] = "STARTED";
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

void Room::handleUpdateFinishGame(int winnerId, int loserId, bool isLeave)
{
    History::createHistory(winnerId, loserId);
    User::updateUserAfterAGame(winnerId, loserId, isLeave);
}

