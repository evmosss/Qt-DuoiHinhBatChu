#include <QString>
#ifndef CONFIG_H
#define CONFIG_H

const QString API_URL =  "http://localhost:8000/api/";

enum RoomDifficulty {
    EASY,
    MEDIUM,
    HARD,
    ADVANCED
};


QString getRoomDifficultyInString(enum RoomDifficulty value);
#endif // CONFIG_H
