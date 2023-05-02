
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
#include "protocolSocket.h"


class Room
{
public:
    // For simple activities
    QJsonObject createRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject joinRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject leaveRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QJsonObject sendAnswer(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId, QString content);

    // For getting question and check result
    QHttpServerResponse startGame(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QHttpServerResponse getAllRoom(QMap<QString, QJsonObject> roomDataMap);

    Room();
};

#endif // ROOM_H
