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

extern int DEFAULT_MAX_QUESTIONS;

#endif // UTIL_H
