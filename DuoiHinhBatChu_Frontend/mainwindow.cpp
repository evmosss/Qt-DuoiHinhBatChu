
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "login.h"
#include "info.h"
#include "register.h"
#include "room.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Tạo hai đối tượng lớp màn hình tương ứng với hai file UI
    Login *loginWidget = new Login(this);
    Register *registerWidget = new Register(this);
    // Info *infoWidget = new Info(this);
    Room *roomWidget = new Room(this);

    // Thêm hai đối tượng vào QStackedWidget
    ui->stackedWidget->addWidget(loginWidget);
    ui->stackedWidget->addWidget(registerWidget);
    ui->stackedWidget->addWidget(roomWidget);

    connect(loginWidget, &Login::changeToRegisterPage, this, &MainWindow::showRegisterWidget);
    connect(loginWidget, &Login::loginSuccessfully, this, &MainWindow::showRoomWidget);
    connect(registerWidget, &Register::registerSuccessfully, this, &MainWindow::showLoginWidget);
    connect(roomWidget, &Room::logOutSuccessfully, this, &MainWindow::showLoginWidget);

    // Chuyển session id từ widget này sang widget khác
    connect(this, &MainWindow::dispatchSessionId, roomWidget, &Room::storeData);

    // Xử lí đóng close window
    connect(this, &MainWindow::closeWindow, roomWidget, &Room::closeWindow);

    // Hiển thị màn hình đăng nhập ban đầu
    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    database.close();
    QSqlDatabase::removeDatabase(database.connectionName());
    delete ui;
}

void MainWindow::showRegisterWidget()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::showLoginWidget()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::showRoomWidget(QString sessionId, int userId)
{
    emit dispatchSessionId(sessionId, userId);
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question(this, "Title", "Bạn có muốn đóng cửa sổ?",
                                                               QMessageBox::No | QMessageBox::Yes,
                                                               QMessageBox::Yes);
    if (resBtn != QMessageBox::Yes) {
        event->ignore();
    } else {
        emit closeWindow();
        event->accept();
    }
}


