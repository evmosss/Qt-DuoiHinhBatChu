export enum RoomStatus {
  PENDING,
  FULL,
  STARTED,
  FINISHED,
}

export enum RoomDifficulty {
  EASY,
  MEDIUM,
  HARD,
  ADVANCED,
}

/**
 * @param players: list of players
 *                 based on their id
 * @param points: list of points relative
 *                to players by index
 * @param questionIndex: the current question
 * @param status: room status
 */
export interface RoomData {
  roomId: string;
  players: [number];
  points: [number];
  ownerId: number;
  status: RoomStatus;
  questionIndex: number;
  maxQuestions: number;
  difficulty: RoomDifficulty;
  winnerId?: number; // Only show up at the end of a match.
}

/**
 * @dev Use to send answer (whether the answer
 *      is true) to all client in a room when
 *      a participant send a answer
 * @param roomDetail: for updating point
 * @param isTrue: check whether the answer
 *                from client is true
 * @param content: message will be appended
 *                 to chat board of that room
 */
export interface SendAnswer {
  roomDetail: RoomData;
  isTrue: boolean;
  content: string;
}
