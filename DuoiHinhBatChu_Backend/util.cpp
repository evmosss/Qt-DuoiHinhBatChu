#include "util.h"

int DEFAULT_MAX_QUESTIONS = 10;

RoomDifficulty getRoomDifficultyFromPoint(int point)
{
    if (point <= 10) {
        return RoomDifficulty::EASY;
    }
    else if (10 < point && point <= 20) {
        return RoomDifficulty::MEDIUM;
    }
    else if (20 < point && point <= 30) {
        return RoomDifficulty::HARD;
    }
    return RoomDifficulty::ADVANCED;
}


bool canJoinRoom(RoomDifficulty roomDiff, int point) {
    RoomDifficulty validRoomDiff = getRoomDifficultyFromPoint(point);
    if (validRoomDiff != roomDiff) {
        return false;
    }
    return true;
}
