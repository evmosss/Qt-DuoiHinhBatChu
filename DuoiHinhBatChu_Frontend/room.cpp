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

    connect(socket, SIGNAL(connected()), this, SLOT(alertConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(handleDataFromServer()));
    connect(socket, SIGNAL(errorOccurred(QAbstractSocket::SocketError)),
            this, SLOT(handleSocketError(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
    connect(this, &Room::interactError, this, &Room::handleInteractError);

    connect(this, &Room::requestAllRoom, this, &Room::sendRequestGetAllRoom);

    connect(this, &Room::getAllRank, this, &Room::requestGetAllRank);


    countdownTimer = new QTimer(this);


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

    qInfo() << this->sessionId;
    if (socket) {
        QJsonObject json;
        json["sessionId"] = sessionId;
        json["type"] = static_cast<int>(SocketType::REQUEST_SAVE_ACTIVE_USER);

        socket->write(convertJsonToByteArray(json));
        socket->flush();
    }
}

void Room::closeWindow()
{
    if (roomId != nullptr) {
        Room::on_leaveRoom_clicked();
        Room::on_logoutButton_clicked();
        emit requestAllRoom(false);
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

    ui->timer->setText("Waiting...");
    ui->questionIndex->setText("Question: 0");

    ui->startButton->setEnabled(false);
    ui->chatView->clear();

    QUrl url("https://media.front.xoedge.com/images/626affda-17e4-4914-82f5-b2cb7b8aa92d~rs_1080.h?fm=webp&q=90");
    downloadImage(url, ui->questionImage);

    emit requestAllRoom(false);
}

void Room::handleLeaveRoom(QJsonObject data)
{
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }
    else {
        if (data.value("data") == QJsonValue::Null) {
            ui->stackedWidget->setCurrentIndex(0);
        }
        else {
            QJsonObject responseData = data.value("data").toObject();
            int questionIndex = responseData.value("questionIndex").toInt();
            qInfo() << "Question Index: " << questionIndex;

            int playerId = responseData.value("leavePlayerId").toInt();
            if (playerId != userId) {
                ui->pointTwo->setText("Point: 0");
                ui->userTwo->setText("Waiting...");
                ui->startButton->setEnabled(false);
                ui->chatView->append("User " + QString::number(playerId) + " has left the game.");
            }
            else {
                ui->chatView->clear();
                ui->stackedWidget->setCurrentIndex(0);
            }
        }
    }

    ui->startButton->setEnabled(true);
    countdownTimer->stop();

    emit requestAllRoom(true);
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
    ui->submitAnswer->setEnabled(true);

    ui->timer->setText("Waiting...");
    QUrl url("https://media.front.xoedge.com/images/626affda-17e4-4914-82f5-b2cb7b8aa92d~rs_1080.h?fm=webp&q=90");
    downloadImage(url, ui->questionImage);

    emit requestAllRoom(false);
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

    emit requestAllRoom(false);
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
    QJsonObject responseData = data.value("data").toObject();
    int winnerId = responseData.value("winnerId").toInt();

    ui->timer->setText("END");
    countdownTimer->stop();

    ui->submitAnswer->setEnabled(false);

    ui->startButton->setEnabled(true);
    ui->chatView->append("User " + QString::number(winnerId) + " win this game.");

    emit requestAllRoom(false);
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
    else {
        ui->startButton->setEnabled(false);
    }
}

void Room::handleGetAllRoom(QJsonObject data)
{
    qInfo() << "Get All Room" << data;
    if (data["code"].toInt() != 200) {
        emit interactError(data["message"].toString());
        return;
    }

    QJsonObject responseData = data.value("data").toObject();
    QList<QString> roomIds = responseData.keys().toList();

    ui->idRoomList->setRowCount(roomIds.size());
    ui->idRoomList->setColumnCount(3);
    ui->idRoomList->setHorizontalHeaderLabels({"Room Id", "Total Player", "Status"});

    QJsonObject roomData;
    int ownerId;
    QJsonArray players;
    QString status;
    QString playerLength;

    if (roomIds.size() > 0) {
        for(int i = 0; i < roomIds.size(); i++) {
            roomData = responseData.value(roomIds.at(i)).toObject();
            players = roomData.value("players").toArray();
            playerLength = QString::number(players.size());
            status = roomData.value("status").toString();

            QTableWidgetItem* item1 = new QTableWidgetItem(roomIds.at(i));
            ui->idRoomList->setItem(i, 0, item1);
            QTableWidgetItem* item3 = new QTableWidgetItem(playerLength);
            ui->idRoomList->setItem(i, 1, item3);
            QTableWidgetItem* item4 = new QTableWidgetItem(status);
            ui->idRoomList->setItem(i, 2, item4);
        }
    }
}

void Room::requestNextQuestion()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["roomId"] = roomId;
    json["type"] = static_cast<int>(SocketType::NEXT_QUESTION);

    socket->write(convertJsonToByteArray(json));
    socket->flush();
}

QByteArray Room::convertJsonToByteArray(QJsonObject val) {
    QJsonDocument jsonDoc(val);
    QByteArray byteArray = jsonDoc.toJson(QJsonDocument::Compact);
    return byteArray + "\n";
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

    socket->write(convertJsonToByteArray(json));
    socket->flush();
}

void Room::on_leaveRoom_clicked()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["roomId"] = roomId;
    json["type"] = static_cast<int>(SocketType::LEAVE_ROOM);

    roomId = nullptr;
    roomOwnerId = 0;

    socket->write(convertJsonToByteArray(json));
    socket->flush();
}

void Room::on_logoutButton_clicked()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["type"] = static_cast<int>(SocketType::REQUEST_DELETE_ACTIVE_USER);

    socket->write(convertJsonToByteArray(json));
    socket->flush();

    sessionId = nullptr;
    emit logOutSuccessfully();
}

void Room::on_joinRoom_clicked()
{
    bool ok = false;
    QString roomId = QInputDialog::getText(this, tr("Enter Room ID"), tr("Room ID:"), QLineEdit::Normal, QString(), &ok);

    if (ok && !roomId.isEmpty()) {
        QJsonObject json;
        json["sessionId"] = sessionId;
        json["type"] = static_cast<int>(SocketType::JOIN_ROOM);
        json["roomId"] = roomId;

        socket->write(convertJsonToByteArray(json));
        socket->flush();
    } else {
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

        socket->write(convertJsonToByteArray(json));
        socket->flush();
    }
}

void Room::on_startButton_clicked()
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["type"] = static_cast<int>(SocketType::START_ROOM);
    json["roomId"] = roomId;

    socket->write(convertJsonToByteArray(json));
    socket->flush();
}

void Room::sendRequestGetAllRoom(bool isSendToCaller)
{
    QJsonObject json;
    json["sessionId"] = sessionId;
    json["isSendToCaller"] = isSendToCaller;
    json["type"] = static_cast<int>(SocketType::SEND_REQUEST_ALL_ROOM);

    qInfo() << "RUN SEND REQUEST GET ALL ROOM";

    socket->write(convertJsonToByteArray(json));
    socket->flush();
}

void Room::requestGetAllRank(int page)
{
    if(page > 0) {
        manager = new QNetworkAccessManager(this);

        QUrl url(API_URL + "rank");
        QUrlQuery query;
        query.addQueryItem("page", QString::number(page));
        query.addQueryItem("limit", QString::number(10));
        url.setQuery(query);

        QNetworkRequest request(url);
        request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

        manager->get(request);

        connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                QByteArray response = reply->readAll();
                QJsonDocument jsonDoc(QJsonDocument::fromJson(response));
                QJsonObject jsonObj = jsonDoc.object();
                QJsonObject jsonData = jsonObj.value("data").toObject();

                if(jsonData.value("data").toArray().size() > 0) {
                    // Xử lý dữ liệu JSON ở đây
                    qInfo() << "[+] DATA FROM SERVER:\n" << jsonData << "\n\n";
                    double resPage = jsonData.value("page").toDouble();
                    double resLimit = jsonData.value("limit").toDouble();
                    QJsonArray resRankList = jsonData.value("data").toArray();

                    ui->stackedWidget->setCurrentWidget(ui->ListRoom);

                    QStringList headerLabels;
                    int row = resRankList.size();
                    QStringList columnKeyList = resRankList.at(0).toObject().keys();
                    headerLabels << "Id" << "Point" << "Total games" << "Win count" << "Win ratio";
                    QFont font;
                    font.setBold(true);
                    ui->rankWidget->setRowCount(row);
                    ui->rankWidget->setColumnCount(columnKeyList.count());
                    ui->rankWidget->setHorizontalHeaderLabels(headerLabels);
                    ui->rankWidget->horizontalHeader()->setFont(font);
                    ui->rankWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
                    ui->rankWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
                    resRankList.size() == resLimit ? ui->rankWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch) : void();
                    for(int i = 0; i < row; i++) {
                        for(int j = 0; j < columnKeyList.count(); j++) {
                            QString key = columnKeyList.at(j);
                            QString itemContent = QString::number(resRankList.at(i).toObject().value(key).toDouble());
                            QTableWidgetItem *item = new QTableWidgetItem(itemContent);
                            item->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
                            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
                            ui->rankWidget->setItem(i, j, item);
                        }
                    }

                    ui->pageValue->setText(QString::number(resPage));
                }
            } else {
                qInfo() << "[+] DATA ERROR \n\n";
            }

            reply->deleteLater();
        });
    }
}

void Room::alertConnected()
{
    // Send message Login to server
    qInfo() << "Socket Connected";
}

void Room::handleDataFromServer()
{
    QByteArray data = socket->readAll();

    QJsonObject jsonData;

    QList<QByteArray> byteArrays = data.split('\n');
    qInfo() << "[+] Byte Array:\n" << byteArrays;

    for (int i = 0; i < byteArrays.size() - 1; i++) {
        jsonData = QJsonDocument::fromJson(byteArrays.at(i)).object();
        qInfo() << "[+] DATA FROM SERVER:\n" << jsonData << "\n\n";

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
        case static_cast<int>(SocketType::REQUEST_ALL_ROOM):
            Room::handleGetAllRoom(jsonData);
            break;
        case static_cast<int>(SocketType::FINISH_SAVE_ACTIVE_USER):
            Room::sendRequestGetAllRoom(true);
            break;
        }
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
    manager = new QNetworkAccessManager(this);

    QNetworkReply* reply = manager->get(QNetworkRequest(imageUrl));

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
        manager->deleteLater();
    });
}

void Room::on_rankBtn_clicked()
{
    emit getAllRank(1);
}


void Room::on_listBackHome_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->MainRoom);
}


void Room::on_previousWidget_clicked()
{
    int page = ui->pageValue->text().toInt() - 1;
    emit getAllRank(page);
}


void Room::on_nextWidget_clicked()
{
    int page = ui->pageValue->text().toInt() + 1;
    emit getAllRank(page);
}

