
#include <QCoreApplication>
#include <QHttpServer>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QList>
#include <QJsonArray>
#include <QStringList>
#include <QRandomGenerator>

// Components
#include "auth.h"
#include "room.h"
#include "user.h"

QMap<QString, QJsonObject> roomDataMap;
QMap<int, QString> userToRoomId;

void createDBConnection(QCoreApplication *a) {
    QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", "SP2");
    database.setDatabaseName("D:/Qt-Dev/altp.db");

    if(!database.open()) {
        qDebug() << "Error: Unable to open database..";
    }

    if (database.open()){
        qInfo() << "Successfully: Connecting to database..";
    }

    QObject::connect(a, &QCoreApplication::aboutToQuit, [&]() {
        database.close();
        QSqlDatabase::removeDatabase(database.connectionName());
    });
}


QString generateRoomId()
{
    const QString alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    const int length = 6;

    QString roomId;
    QRandomGenerator generator = QRandomGenerator::securelySeeded();

    for (int i = 0; i < length; ++i) {
        const int index = generator.bounded(alphabet.length());
        roomId.append(alphabet.at(index));
    }

    return roomId;
}

QString getSessionIdFromHeader(const QHttpServerRequest &req) {
    QList<QPair<QByteArray, QByteArray>> headers = req.headers();
    for (const QPair<QByteArray, QByteArray>& header : headers) {
        if (header.first == "Authorization") {
            QString bearerToken = QString(header.second).replace("Bearer ", "");
            // sử dụng bearerToken ở đây để thực hiện các hoạt động khác
            return bearerToken;
        }
    }
    return nullptr;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Auth* authService = new Auth;
    Room* roomService = new Room;
    User* userService = new User;

    createDBConnection(&a);

    QHttpServer server;


    // Auth Apis
    server.route("/api/auth/login", QHttpServerRequest::Method::Post, [authService](const QHttpServerRequest &req) {
        QByteArray requestData = req.body();
        QJsonObject json = QJsonDocument::fromJson(requestData).object();

        // Lấy tài khoản và mật khẩu từ yêu cầu Api
        QString username = json.value("username").toString();
        QString password = json.value("password").toString();

        qInfo() << username << password;

        return authService->login(&username, &password);
    });

    server.route("/api/auth/register", QHttpServerRequest::Method::Post, [authService](const QHttpServerRequest &req) {
        QByteArray requestData = req.body();
        QJsonObject json = QJsonDocument::fromJson(requestData).object();

        // Lấy tài khoản và mật khẩu từ yêu cầu Api
        QString username = json.value("username").toString();
        QString password = json.value("password").toString();

        return authService->registerAccount(&username, &password);
    });

    // Room Apis
    server.route("/api/room/create", QHttpServerRequest::Method::Post, [roomService, userService](const QHttpServerRequest &req) {
        QByteArray requestData = req.body();

        QString sessionId = getSessionIdFromHeader(req);

        if (sessionId == nullptr) {
            return QHttpServerResponse(QJsonObject{ {"message", "Lack of session id"} }, QHttpServerResponder::StatusCode::BadRequest);
        }

        int userId = userService->getUserFromSessionId(&sessionId);
        if (userId == 0) {
            return QHttpServerResponse(QJsonObject{ {"message", "Please login again to get correct session id"} }, QHttpServerResponder::StatusCode::Forbidden);
        }

        QString roomId = nullptr;
        do {
            roomId = generateRoomId();
        } while (roomDataMap.contains(roomId));

        return roomService->createRoom(userId, &roomDataMap, &roomId, &userToRoomId);
    });
    server.route("/api/room/join", QHttpServerRequest::Method::Post, [roomService, userService](const QHttpServerRequest &req) {
        QByteArray requestData = req.body();
        QJsonObject json = QJsonDocument::fromJson(requestData).object();

        QString sessionId = getSessionIdFromHeader(req);

        if (sessionId == nullptr) {
            return QHttpServerResponse(QJsonObject{ {"message", "Lack of session id"} }, QHttpServerResponder::StatusCode::BadRequest);
        }

        int userId = userService->getUserFromSessionId(&sessionId);
        if (userId == 0) {
            return QHttpServerResponse(QJsonObject{ {"message", "Please login again to get correct session id"} }, QHttpServerResponder::StatusCode::Forbidden);
        }

        QString roomId = json.value("roomId").toString();

        return roomService->joinRoom(userId, &roomDataMap, &roomId, &userToRoomId);
    });
    server.route("/api/room/leave", QHttpServerRequest::Method::Post, [roomService, userService](const QHttpServerRequest &req) {
        QByteArray requestData = req.body();
        QJsonObject json = QJsonDocument::fromJson(requestData).object();

        QString sessionId = getSessionIdFromHeader(req);

        if (sessionId == nullptr) {
            return QHttpServerResponse(QJsonObject{ {"message", "Lack of session id"} }, QHttpServerResponder::StatusCode::BadRequest);
        }

        int userId = userService->getUserFromSessionId(&sessionId);
        if (userId == 0) {
            return QHttpServerResponse(QJsonObject{ {"message", "Please login again to get correct session id"} }, QHttpServerResponder::StatusCode::Forbidden);
        }

        QString roomId = json.value("roomId").toString();

        return roomService->leaveRoom(userId, &roomDataMap, &roomId, &userToRoomId);
    });
    server.route("/api/room/start", QHttpServerRequest::Method::Post, [roomService, userService](const QHttpServerRequest &req) {
        QByteArray requestData = req.body();
        QJsonObject json = QJsonDocument::fromJson(requestData).object();

        QString sessionId = getSessionIdFromHeader(req);

        if (sessionId == nullptr) {
            return QHttpServerResponse(QJsonObject{ {"message", "Lack of session id"} }, QHttpServerResponder::StatusCode::BadRequest);
        }

        int userId = userService->getUserFromSessionId(&sessionId);
        if (userId == 0) {
            return QHttpServerResponse(QJsonObject{ {"message", "Please login again to get correct session id"} }, QHttpServerResponder::StatusCode::Forbidden);
        }

        QString roomId = json.value("roomId").toString();

        return roomService->startGame(userId, &roomDataMap, &roomId, &userToRoomId);
    });


    server.route("/api/room/all", QHttpServerRequest::Method::Get, [roomService](const QHttpServerRequest &req) {
        QByteArray requestData = req.body();

        return roomService->getAllRoom(roomDataMap);
    });


    server.listen(QHostAddress::LocalHost, 8000);


    return a.exec();
}
