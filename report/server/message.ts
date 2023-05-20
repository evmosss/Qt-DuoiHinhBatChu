import { SocketType } from "../socket"
import { RoomData, SendAnswer } from "./room"

/**
 * @param code: http status code
 * @param message: popping up message 
 *                 purpose
 * @param type: detecting socket type 
 *              to handle
 * @param data: has room type or null 
 *              if it is error case
 */
export interface Message {
    code: number,
    message: string,
    type: SocketType,
    data: RoomData | SendAnswer | null 
}
