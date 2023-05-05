#include "room.h"
#include "ui_room.h"
#include "socket.h"

Room::Room(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Room)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentWidget(ui->MainRoom);

    socket = Socket::getInstance().socket();
    socket->connectToHost("127.0.0.1", 9454);

    connect(socket,SIGNAL(connected()),this,SLOT(alertConnected()));
    connect(socket,SIGNAL(readyRead()),this,SLOT(handleDataFromServer()));
    connect(socket,SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this,SLOT(handleSocketError(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnect()));
    connect(this, &Room::interactError, this, &Room::handleInteractError);

    countdownTimer = new QTimer(this);

    // Kết nối khe hở QTimer::timeout() để giảm giá trị đếm ngược và cập nhật QLabel
    connect(countdownTimer, &QTimer::timeout, [=]() {
        int currentValue = ui->timer->text().toInt();
        if (currentValue > 0) {
            ui->timer->setText(QString::number(currentValue - 1));
        } else {
            if (userId == roomOwnerId) {
                requestNextQuestion();
            }
            countdownTimer->stop();
        }
    });
}

Room::~Room()
{
    socket->disconnectFromHost();
    delete ui;
}

void Room::storeData(QString sessionId, int userId)
{
    this->sessionId = sessionId;
    this->userId = userId;
}

void Room::closeWindow()
{
    if (roomId != nullptr) {
        Room::on_leaveRoom_clicked();
    }
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
    roomOwnerId = userData.value("ownerId").toInt();

    qInfo() << playerData.at(0);
    QString pointStr = "Point: " + QString::number(pointData.at(0).toDouble());
    QString userStr = "User: " + QString::number(playerData.at(0).toDouble());

    ui->stackedWidget->setCurrentWidget(ui->RoomDetail);
    ui->pointOne->setText(pointStr);
    ui->userOne->setText(userStr);
    ui->questionIndex->setText("Question: " + QString::number(userData.value("questionIndex").toInt()));
    ui->roomId->setText("Room id: " + roomId);

    ui->stackedWidget->setCurrentWidget(ui->RoomDetail);
    ui->pointTwo->setText("Point: 0");
    ui->userTwo->setText("Waiting...");

    ui->startButton->setEnabled(false);
    ui->chatView->clear();
}

void Room::handleLeaveRoom(QJsonObject data)
{
    qInfo() << "Leave Room" << data;
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }
    else {
        qInfo() << "Touch here";
        if (data.value("data") == QJsonValue::Null) {
            ui->stackedWidget->setCurrentIndex(0);
        }
        else {
            QJsonObject responseData = data.value("data").toObject();
            int playerId = responseData.value("leavePlayerId").toInt();
            if (playerId != userId) {
                ui->pointTwo->setText("Point: 0");
                ui->userTwo->setText("Waiting...");
                ui->startButton->setEnabled(false);
            }
            else {
                ui->stackedWidget->setCurrentIndex(0);
            }
        }
    }
    ui->chatView->clear();
}

void Room::handleJoinRoom(QJsonObject data)
{
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }

    QJsonObject roomDetail = data.value("data").toObject();
    renderFullRoom(roomDetail);

    ui->chatView->clear();
}

void Room::handleSendAnswer(QJsonObject data)
{
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }

    QJsonObject userData = data.value("data").toObject();
    QString content = userData.value("content").toString();
    bool isTrue = userData.value("isTrue").toBool();
    QJsonObject roomDetail = userData.value("roomDetail").toObject();
    renderFullRoom(roomDetail);

    QString sendData = content;
    if (isTrue) {
        sendData = sendData + " (Correct)";
        ui->submitAnswer->setEnabled(false);

        if (userId == roomOwnerId) {
            requestNextQuestion();
        }
    }

    ui->chatView->append(sendData);

    if (isTrue) {
        ui->chatView->append("Wait until next question...");
    }

    ui->inputAnswer->clear();
    ui->inputAnswer->focusWidget();
}

void Room::handleStartRoom(QJsonObject data)
{
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }

    QJsonObject responseData = data.value("data").toObject();
    QJsonObject questionData = responseData.value("questionData").toObject();
    QString imageUrl = questionData.value("image_url").toString();
    int questionIndex = responseData.value("questionIndex").toInt();

    QUrl url(imageUrl);

    downloadImage(url, ui->questionImage);

    ui->startButton->setEnabled(false);
    ui->questionIndex->setText("Question: " + QString::number(questionIndex));
    ui->timer->setText("60");
    ui->submitAnswer->setEnabled(true);
    countdownTimer->start(1000); // Đếm ngược mỗi giây
}

void Room::handleNextQuestion(QJsonObject data)
{
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }

    QJsonObject responseData = data.value("data").toObject();
    QJsonObject questionData = responseData.value("questionData").toObject();
    QString imageUrl = questionData.value("image_url").toString();
    int questionIndex = responseData.value("questionIndex").toInt();
    int maxQuestion = responseData.value("maxQuestions").toInt();
    QUrl url(imageUrl);
    downloadImage(url, ui->questionImage);

    if (questionIndex > maxQuestion) {
        ui->timer->setText("END");
        countdownTimer->stop();
        ui->submitAnswer->setEnabled(false);
        ui->questionIndex->setText("Question: " + QString::number(questionIndex));
    }
    else {
        ui->submitAnswer->setEnabled(true);
        ui->timer->setText("60");
        ui->questionIndex->setText("Question: " + QString::number(questionIndex));
        countdownTimer->start(1000); // Đếm ngược mỗi giây
    }
}

void Room::handleFinishRoom(QJsonObject data)
{
    qInfo() << "RUN HERE BRO THIS IS FOR FINISHING A ROOM" << data;
    QJsonObject responseData = data.value("data").toObject();
    int winnerId = responseData.value("winnerId").toInt();

    ui->timer->setText("END");
    countdownTimer->stop();
    ui->submitAnswer->setEnabled(false);

    ui->startButton->setEnabled(true);
    ui->chatView->append("User " + QString::number(winnerId) + " win this game.");
}


void Room::renderFullRoom(QJsonObject roomDetail) {
    QJsonArray playerData = roomDetail.value("players").toArray();
    QJsonArray pointData = roomDetail.value("points").toArray();
    roomId = roomDetail.value("roomId").toString();

    // Set up for first player
    QString pointStr = "Point: " + QString::number(pointData.at(0).toDouble());
    QString userStr = "User: " + QString::number(playerData.at(0).toDouble());

    ui->stackedWidget->setCurrentWidget(ui->RoomDetail);
    ui->pointOne->setText(pointStr);
    ui->userOne->setText(userStr);
    ui->questionIndex->setText("Question: " + QString::number(roomDetail.value("questionIndex").toInt()));
    ui->roomId->setText("Room id: " + roomId);

    // Set up for second player
    ui->stackedWidget->setCurrentWidget(ui->RoomDetail);
    ui->pointTwo->setText("Point: " + QString::number(pointData.at(1).toDouble()));
    ui->userTwo->setText("User: " + QString::number(playerData.at(1).toDouble()));

    // Only owner can start game
    if (userId == playerData.at(0).toInt()) {
        ui->startButton->setEnabled(true);
    }
}

void Room::requestNextQuestion()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["roomId"] = roomId;
    json["type"] = static_cast<int>(SocketType::NEXT_QUESTION);

    QJsonDocument jsonDoc(json);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    socket->write(jsonString.toUtf8());
    socket->flush();
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
    roomId = nullptr;
    roomOwnerId = 0;

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

void Room::on_submitAnswer_clicked()
{
    QString text = ui->inputAnswer->text();

    if (text != "") {
        // Send message to server
        QJsonObject json;
        json["sessionId"] = sessionId;
        json["type"] = static_cast<int>(SocketType::SEND_ANSWER);
        json["roomId"] = roomId;
        json["content"] = text;

        QJsonDocument jsonDoc(json);
        QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

        socket->write(jsonString.toUtf8());
        socket->flush();
    }
}

void Room::on_startButton_clicked()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["type"] = static_cast<int>(SocketType::START_ROOM);
    json["roomId"] = roomId;

    QJsonDocument jsonDoc(json);
    QString jsonString = jsonDoc.toJson(QJsonDocument::Compact);

    socket->write(jsonString.toUtf8());
    socket->flush();
}

void Room::alertConnected()
{
    qInfo() << "Connect to socket server successfully";
}

void Room::handleDataFromServer()
{
    QByteArray data = socket->readAll();
    QJsonObject jsonData = QJsonDocument::fromJson(data).object();
    qInfo() << "DATA FROM SERVER" << jsonData;

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
        case static_cast<int>(SocketType::SEND_ANSWER):
        Room::handleSendAnswer(jsonData);
        break;
        case static_cast<int>(SocketType::START_ROOM):
        Room::handleStartRoom(jsonData);
        break;
        case static_cast<int>(SocketType::NEXT_QUESTION):
        Room::handleNextQuestion(jsonData);
        break;
        case static_cast<int>(SocketType::FINISH_ROOM):
        Room::handleFinishRoom(jsonData);
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

void Room::downloadImage(const QUrl& imageUrl, QLabel* label)
{
    QNetworkAccessManager* networkManager = new QNetworkAccessManager();

    QNetworkReply* reply = networkManager->get(QNetworkRequest(imageUrl));

    QObject::connect(reply, &QNetworkReply::finished, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QPixmap pixmap;
            pixmap.loadFromData(data);
            label->setPixmap(pixmap);
        } else {
            qDebug() << "Error: " << reply->errorString();
        }

        reply->deleteLater();
        networkManager->deleteLater();
    });
}
