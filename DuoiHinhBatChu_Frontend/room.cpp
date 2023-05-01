#include "room.h"
#include "ui_room.h"

Room::Room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->MainRoom);

    socket = new QTcpSocket();
    socket->connectToHost("127.0.0.1", 9454);

    connect(socket,SIGNAL(connected()),this,SLOT(alertConnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(handleDataFromServer()));
    connect(socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this,SLOT(handleSocketError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnect()));
    connect(this, &Room::interactError, this, &Room::handleInteractError);
}

Room::~Room()
{
    socket->disconnectFromHost();
    delete ui;
}

void Room::storeSessionId(QString sessionId)
{
    this->sessionId = sessionId;
}

void Room::handleCreateRoom(QJsonObject data)
{
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }

    QJsonObject userData = data.value("data").toObject();
    QJsonArray playerData = userData.value("players").toArray();
    QJsonArray pointData = userData.value("points").toArray();
    roomId = userData.value("roomId").toString();

    qInfo() << playerData.at(0);
    QString pointStr = "Point: " + QString::number(pointData.at(0).toDouble());
    QString userStr = "User: " + QString::number(playerData.at(0).toDouble());

    ui->stackedWidget->setCurrentWidget(ui->RoomDetail);
    ui->pointOne->setText(pointStr);
    ui->userOne->setText(userStr);
    ui->questionIndex->setText("Question: " + QString::number(userData.value("questionIndex").toInt()));
    ui->roomId->setText("Room id: " + roomId);
    ui->startButton->setDisabled(false);
}

void Room::handleLeaveRoom(QJsonObject data)
{
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }
    else {
        qInfo() << "Touch here";
        ui->stackedWidget->setCurrentIndex(0);
        // Room::disconnect();
    }
}

void Room::handleJoinRoom(QJsonObject data)
{
    qInfo() << "This is data for join room" << data;
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }

    QJsonObject userData = data.value("data").toObject();
    QJsonArray playerData = userData.value("players").toArray();
    QJsonArray pointData = userData.value("points").toArray();
    roomId = userData.value("roomId").toString();

    // Set up for first player
    QString pointStr = "Point: " + QString::number(pointData.at(0).toDouble());
    QString userStr = "User: " + QString::number(playerData.at(0).toDouble());

    ui->stackedWidget->setCurrentWidget(ui->RoomDetail);
    ui->pointOne->setText(pointStr);
    ui->userOne->setText(userStr);
    ui->questionIndex->setText("Question: " + QString::number(userData.value("questionIndex").toInt()));
    ui->roomId->setText("Room id: " + roomId);

    // Set up for second player
    ui->stackedWidget->setCurrentWidget(ui->RoomDetail);
    ui->pointTwo->setText("Point: " + QString::number(pointData.at(1).toDouble()));
    ui->userTwo->setText("User: " + QString::number(playerData.at(1).toDouble()));

    ui->startButton->setDisabled(false);
}

void Room::handleInteractError(QString message)
{
    qDebug() << "INTERACTIVE ERROR:" << message;
    QMessageBox::critical(this, "Error", message);
}

void Room::on_createRoom_clicked()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["type"] = static_cast<int>(SocketType::CREATE_ROOM);

    QJsonDocument jsonDoc(json);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    socket->write(jsonString.toUtf8());
    socket->flush();
}

void Room::on_leaveRoom_clicked()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["roomId"] = roomId;
    json["type"] = static_cast<int>(SocketType::LEAVE_ROOM);

    QJsonDocument jsonDoc(json);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    socket->write(jsonString.toUtf8());
    socket->flush();
}

void Room::on_logoutButton_clicked()
{
    sessionId = nullptr;
    emit logOutSuccessfully();
}

void Room::on_joinRoom_clicked()
{
    bool ok = false;
    QString roomId = QInputDialog::getText(this, tr("Enter Room ID"), tr("Room ID:"), QLineEdit::Normal, QString(), &ok);

    if (ok && !roomId.isEmpty()) {
        // User clicked OK and entered text, handle the roomId
        qInfo() << "User entered room ID:" << roomId;
        QJsonObject json;
        json["sessionId"] = sessionId;
        json["type"] = static_cast<int>(SocketType::JOIN_ROOM);
        json["roomId"] = roomId;

        QJsonDocument jsonDoc(json);
        QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

        socket->write(jsonString.toUtf8());
        socket->flush();
    } else {
        // User clicked Cancel or didn't enter any text
        qInfo() << "User cancelled or didn't enter a room ID";
    }
}

void Room::alertConnected()
{
    qInfo() << "Connect to socket server successfully";
}

void Room::handleDataFromServer()
{
    QByteArray data = socket->readAll();
    QJsonObject jsonData = QJsonDocument::fromJson(data).object();

    switch (jsonData["type"].toInt()) {
        case static_cast<int>(SocketType::CREATE_ROOM):
        Room::handleCreateRoom(jsonData);
        break;
        case static_cast<int>(SocketType::LEAVE_ROOM):
        Room::handleLeaveRoom(jsonData);
        break;
        case static_cast<int>(SocketType::JOIN_ROOM):
        Room::handleJoinRoom(jsonData);
        break;
    }
}

void Room::handleSocketError(QAbstractSocket::SocketError error)
{
    qInfo() << "Error Occurred! Error code: " + QString::number(error);
}

void Room::disconnect()
{
    if(socket->isOpen())
    {
        socket->close();
        socket->deleteLater();
    }
}
