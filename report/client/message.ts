import { SocketType } from "../socket";

/**
 * @param sessionId: session getting from login
 * @param roomId: room id
 * @param type: detecting socket type to handle
 */
export interface Message {
    sessionId: string,
    type: SocketType,
    roomId: string
}