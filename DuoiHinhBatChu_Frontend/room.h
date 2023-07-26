#ifndef ROOM_H
#define ROOM_H

#include <QWidget>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QJsonObject>
#include <QJsonDocument>
#include "protocolSocket.h"
#include <QJsonArray>
#include <QString>
#include <QInputDialog>
#include <QTimer>
#include <QMessageBox>
#include <QUrl>
#include <QUrlQuery>
#include "socket.h"
#include "networkmanager.h"
#include "config.h"

namespace Ui {
class Room;
}

class Room : public QWidget
{
    Q_OBJECT

public:
    explicit Room(QWidget *parent = nullptr);
    ~Room();
    QJsonObject currentQuestionData;
    QString sessionId;
    QString roomId;
    int roomOwnerId;
    int userId;
    void storeData(QString sessionId, int userId);
    void closeWindow();
    void downloadImage(const QUrl& imageUrl, QLabel* label);

private:
    void handleCreateRoom(QJsonObject data);
    void handleLeaveRoom(QJsonObject data);
    void handleJoinRoom(QJsonObject data);
    void handleSendAnswer(QJsonObject data);
    void handleStartRoom(QJsonObject data);
    void handleNextQuestion(QJsonObject data);
    void handleFinishRoom(QJsonObject data);
    void renderFullRoom(QJsonObject roomDetail);
    void handleGetAllRoom(QJsonObject data);
    void handleRequestGetUserInfo(QJsonObject data);
    void forceLogout();

    void requestNextQuestion();

    QByteArray convertJsonToByteArray(QJsonObject val);

    Ui::Room *ui;
    QTcpSocket * socket;
    QTimer* countdownTimer;
    QNetworkAccessManager *manager;

signals:
    void interactError(QString message);
    void logOutSuccessfully();

    void requestAllRoom(bool isSendToCaller);          // emit to load room


    void getAllRank(int page);  // get all rank in number page
    void gettUserInfo(int userId);    // get user info

private slots:
    void handleInteractError(QString message);
    void on_createRoom_clicked();
    void on_leaveRoom_clicked();
    void on_logoutButton_clicked();
    void on_joinRoom_clicked();
    void on_submitAnswer_clicked();
    void on_startButton_clicked();
    void sendRequestGetAllRoom(bool isSendToCaller);
    void requestGetAllRank(int page);   // request to get all rank
    void requestGetUserInfo(int userId);    // request to get userInfo

    void alertConnected();
    void handleDataFromServer();
    void handleSocketError(QAbstractSocket::SocketError error);
    void disconnect(); // Disconnect from server

    void on_rankBtn_clicked();
    void on_listBackHome_clicked();
    void on_previousWidget_clicked();
    void on_nextWidget_clicked();

    void on_playWithComputerBtn_clicked();
    void on_leaveTrain_clicked();
    void on_nextQTrain_clicked();
    void on_getQTrain_clicked();
    void on_submitTrain_clicked();
};

#endif // ROOM_H
