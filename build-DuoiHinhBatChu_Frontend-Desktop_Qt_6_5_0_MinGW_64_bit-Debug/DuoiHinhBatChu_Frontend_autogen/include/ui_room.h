/********************************************************************************
** Form generated from reading UI file 'room.ui'
**
** Created by: Qt User Interface Compiler version 6.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROOM_H
#define UI_ROOM_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Room
{
public:
    QStackedWidget *stackedWidget;
    QWidget *MainRoom;
    QLabel *label;
    QPushButton *createRoom;
    QPushButton *joinRoom;
    QPushButton *logoutButton;
    QWidget *ListRoom;
    QWidget *RoomDetail;
    QLabel *questionImage;
    QPushButton *leaveRoom;
    QTextEdit *chatView;
    QLabel *timer;
    QLineEdit *inputAnswer;
    QPushButton *submitAnswer;
    QLabel *playerOneAvt;
    QLabel *playerTwoAvt;
    QLabel *userOne;
    QLabel *userTwo;
    QLabel *pointOne;
    QLabel *pointTwo;
    QLabel *questionIndex;
    QPushButton *startButton;
    QLabel *roomId;

    void setupUi(QWidget *Room)
    {
        if (Room->objectName().isEmpty())
            Room->setObjectName("Room");
        Room->resize(800, 600);
        stackedWidget = new QStackedWidget(Room);
        stackedWidget->setObjectName("stackedWidget");
        stackedWidget->setGeometry(QRect(0, 10, 800, 600));
        MainRoom = new QWidget();
        MainRoom->setObjectName("MainRoom");
        label = new QLabel(MainRoom);
        label->setObjectName("label");
        label->setGeometry(QRect(250, 190, 301, 31));
        createRoom = new QPushButton(MainRoom);
        createRoom->setObjectName("createRoom");
        createRoom->setGeometry(QRect(240, 260, 311, 61));
        joinRoom = new QPushButton(MainRoom);
        joinRoom->setObjectName("joinRoom");
        joinRoom->setGeometry(QRect(240, 340, 311, 61));
        logoutButton = new QPushButton(MainRoom);
        logoutButton->setObjectName("logoutButton");
        logoutButton->setGeometry(QRect(620, 30, 161, 51));
        stackedWidget->addWidget(MainRoom);
        ListRoom = new QWidget();
        ListRoom->setObjectName("ListRoom");
        stackedWidget->addWidget(ListRoom);
        RoomDetail = new QWidget();
        RoomDetail->setObjectName("RoomDetail");
        questionImage = new QLabel(RoomDetail);
        questionImage->setObjectName("questionImage");
        questionImage->setGeometry(QRect(160, 10, 471, 301));
        questionImage->setPixmap(QPixmap(QString::fromUtf8("../images/waiting_image.webp")));
        questionImage->setScaledContents(true);
        leaveRoom = new QPushButton(RoomDetail);
        leaveRoom->setObjectName("leaveRoom");
        leaveRoom->setGeometry(QRect(0, 10, 83, 29));
        chatView = new QTextEdit(RoomDetail);
        chatView->setObjectName("chatView");
        chatView->setGeometry(QRect(120, 330, 571, 211));
        timer = new QLabel(RoomDetail);
        timer->setObjectName("timer");
        timer->setGeometry(QRect(660, 90, 111, 31));
        timer->setStyleSheet(QString::fromUtf8("font-size: 24px; color: black;"));
        timer->setScaledContents(false);
        timer->setAlignment(Qt::AlignCenter);
        inputAnswer = new QLineEdit(RoomDetail);
        inputAnswer->setObjectName("inputAnswer");
        inputAnswer->setGeometry(QRect(120, 550, 451, 28));
        submitAnswer = new QPushButton(RoomDetail);
        submitAnswer->setObjectName("submitAnswer");
        submitAnswer->setGeometry(QRect(590, 550, 101, 29));
        playerOneAvt = new QLabel(RoomDetail);
        playerOneAvt->setObjectName("playerOneAvt");
        playerOneAvt->setGeometry(QRect(10, 490, 91, 91));
        playerOneAvt->setPixmap(QPixmap(QString::fromUtf8("C:/Users/USER/Desktop/30soccer-ronaldo-1-76fd-mobileMasterAt3x.jpg")));
        playerOneAvt->setScaledContents(true);
        playerTwoAvt = new QLabel(RoomDetail);
        playerTwoAvt->setObjectName("playerTwoAvt");
        playerTwoAvt->setGeometry(QRect(700, 490, 91, 91));
        playerTwoAvt->setPixmap(QPixmap(QString::fromUtf8("C:/Users/USER/Desktop/avatar-1606916__341.jpg")));
        playerTwoAvt->setScaledContents(true);
        userOne = new QLabel(RoomDetail);
        userOne->setObjectName("userOne");
        userOne->setGeometry(QRect(10, 460, 91, 20));
        userOne->setAlignment(Qt::AlignCenter);
        userTwo = new QLabel(RoomDetail);
        userTwo->setObjectName("userTwo");
        userTwo->setGeometry(QRect(700, 460, 91, 20));
        userTwo->setAlignment(Qt::AlignCenter);
        pointOne = new QLabel(RoomDetail);
        pointOne->setObjectName("pointOne");
        pointOne->setGeometry(QRect(10, 430, 91, 20));
        pointOne->setAlignment(Qt::AlignCenter);
        pointTwo = new QLabel(RoomDetail);
        pointTwo->setObjectName("pointTwo");
        pointTwo->setGeometry(QRect(700, 430, 91, 20));
        pointTwo->setAlignment(Qt::AlignCenter);
        questionIndex = new QLabel(RoomDetail);
        questionIndex->setObjectName("questionIndex");
        questionIndex->setGeometry(QRect(670, 140, 111, 31));
        questionIndex->setStyleSheet(QString::fromUtf8("font-size: 16px; color: black;"));
        questionIndex->setScaledContents(false);
        questionIndex->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        startButton = new QPushButton(RoomDetail);
        startButton->setObjectName("startButton");
        startButton->setGeometry(QRect(10, 330, 101, 41));
        roomId = new QLabel(RoomDetail);
        roomId->setObjectName("roomId");
        roomId->setGeometry(QRect(0, 60, 141, 31));
        stackedWidget->addWidget(RoomDetail);

        retranslateUi(Room);

        stackedWidget->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(Room);
    } // setupUi

    void retranslateUi(QWidget *Room)
    {
        Room->setWindowTitle(QCoreApplication::translate("Room", "Form", nullptr));
        label->setText(QCoreApplication::translate("Room", "Welcome To Our Project (Huy + Dang + Khoi)", nullptr));
        createRoom->setText(QCoreApplication::translate("Room", "Create Room", nullptr));
        joinRoom->setText(QCoreApplication::translate("Room", "Join Room", nullptr));
        logoutButton->setText(QCoreApplication::translate("Room", "Logout", nullptr));
        questionImage->setText(QString());
        leaveRoom->setText(QCoreApplication::translate("Room", "Leave", nullptr));
        timer->setText(QCoreApplication::translate("Room", "Time: 9s", nullptr));
        inputAnswer->setPlaceholderText(QCoreApplication::translate("Room", "Input your answer", nullptr));
        submitAnswer->setText(QCoreApplication::translate("Room", "Submit", nullptr));
        playerOneAvt->setText(QString());
        playerTwoAvt->setText(QString());
        userOne->setText(QCoreApplication::translate("Room", "User 1", nullptr));
        userTwo->setText(QCoreApplication::translate("Room", "Waiting...", nullptr));
        pointOne->setText(QCoreApplication::translate("Room", "Point: 0", nullptr));
        pointTwo->setText(QCoreApplication::translate("Room", "Point: 0", nullptr));
        questionIndex->setText(QCoreApplication::translate("Room", "Question: 0", nullptr));
        startButton->setText(QCoreApplication::translate("Room", "Start", nullptr));
        roomId->setText(QCoreApplication::translate("Room", "Room id:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Room: public Ui_Room {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROOM_H
