#include "config.h"

QString getRoomDifficultyInString(enum RoomDifficulty value) {
    switch (value) {
    case RoomDifficulty::EASY:
        return "Easy";
    case RoomDifficulty::MEDIUM:
        return "Medium";
    case RoomDifficulty::HARD:
        return "Hard";
    case RoomDifficulty::ADVANCED:
        return "Advance";
    }
}
