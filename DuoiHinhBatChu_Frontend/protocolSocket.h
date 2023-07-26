
#ifndef PROTOCOLSOCKET_H
#define PROTOCOLSOCKET_H

enum SocketType {
    CREATE_ROOM, // 0
    JOIN_ROOM, // 1
    LEAVE_ROOM, // 2
    START_ROOM, // 3
    NEXT_QUESTION, // 4
    SEND_ANSWER, // 5
    BAD_RESPONSE, // 6
    FINISH_ROOM, // 7
    REQUEST_ALL_ROOM, // 8
    SEND_REQUEST_ALL_ROOM, // 9
    REQUEST_SAVE_ACTIVE_USER, // 10
    REQUEST_DELETE_ACTIVE_USER, // 11
    FINISH_SAVE_ACTIVE_USER, // 12
    GET_USER_INFO   // 13
};

#endif // PROTOCOLSOCKET_H
