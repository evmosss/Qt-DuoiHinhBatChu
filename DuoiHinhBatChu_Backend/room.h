
#ifndef ROOM_H
#define ROOM_H
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QObject>
#include <QDateTime>
#include "protocolSocket.h"
#include "database.h"
#include "history.h"
#include "user.h"


class Room
{
public:
    // Helpers
    int findUserInArray(int userId, QJsonArray players);
    // For simple activities
    QJsonObject createRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject joinRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject leaveRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject sendAnswer(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId, QString content);
    QJsonObject startRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject nextQuestion(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject finishGame(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject getAllCurrentRoom(int userId, QMap<QString, QJsonObject> roomData);

    // For getting question and check result
    QHttpServerResponse startGame(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QHttpServerResponse getAllRoom(QMap<QString, QJsonObject> roomDataMap);

    Room();
private:
    void handleUpdateFinishGame(int winnerId, int loserId, bool isLeave);
};

#endif // ROOM_H
