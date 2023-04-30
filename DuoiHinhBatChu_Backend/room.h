
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


class Room
{
public:
    // For simple activities
    QHttpServerResponse createRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QHttpServerResponse joinRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QHttpServerResponse leaveRoom(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);

    // For getting question and check result
    QHttpServerResponse startGame(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    QHttpServerResponse getAllRoom(QMap<QString, QJsonObject> roomDataMap);

    Room();
};

#endif // ROOM_H
