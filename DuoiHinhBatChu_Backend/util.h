#include <QHttpServerResponse>
#include <QJsonObject>
#include <QString>
#include "protocolSocket.h"

#ifndef UTIL_H
#define UTIL_H

enum RoomDifficulty {
    EASY,
    MEDIUM,
    HARD,
    ADVANCED
};

enum RoomDifficulty getRoomDifficultyFromPoint(int point);
bool canJoinRoom(RoomDifficulty roomDiff, int point);
QJsonObject createSocketResponse(QJsonObject data, QHttpServerResponse::StatusCode status, QString message, SocketType type);

extern int DEFAULT_MAX_QUESTIONS;

#endif // UTIL_H
