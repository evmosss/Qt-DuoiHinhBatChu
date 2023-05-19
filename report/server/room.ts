
export enum RoomStatus {
    PENDING,
    FULL,
    STARTED,
    FINISHED,
}

/**
 * @param players: list of players based on their id
 * @param points: list of points relative to players by index
 * @param questionIndex: the current question
 * @param status: room status
 */
export interface RoomData {
    roomId: string,
    players: [number],
    points: [number],
    ownerId: number,
    status: RoomStatus,
    questionIndex: number,
    maxQuestions: number,
}
