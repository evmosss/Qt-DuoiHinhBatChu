#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include "protocolSocket.h"
#include <QJsonArray>
#include <QString>
#include <QInputDialog>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class Room;
}

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = nullptr);
    ~Room();
    QString sessionId;
    QString roomId;
    int userId;
    void storeData(QString sessionId, int userId);
    void closeWindow();

private:
    void handleCreateRoom(QJsonObject data);
    void handleLeaveRoom(QJsonObject data);
    void handleJoinRoom(QJsonObject data);
    void handleSendAnswer(QJsonObject data);

    Ui::Room *ui;
    QTcpSocket *socket;

signals:
    void interactError(QString message);
    void logOutSuccessfully();

private slots:
    void handleInteractError(QString message);
    void on_createRoom_clicked();
    void on_leaveRoom_clicked();
    void on_logoutButton_clicked();
    void on_joinRoom_clicked();
    void on_submitAnswer_clicked();

    void alertConnected();
    void handleDataFromServer();
    void handleSocketError(QAbstractSocket::SocketError error);
    void disconnect(); // Disconnect from server

};

#endif // ROOM_H
