
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "login.h"
#include "info.h"
#include "register.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Tạo hai đối tượng lớp màn hình tương ứng với hai file UI
    Login *loginWidget = new Login(this);
    Register *registerWidget = new Register(this);
    Info *infoWidget = new Info(this);

    // Thêm hai đối tượng vào QStackedWidget
    ui->stackedWidget->addWidget(loginWidget);
    ui->stackedWidget->addWidget(registerWidget);
    ui->stackedWidget->addWidget(infoWidget);

    connect(loginWidget, &Login::changeToRegisterPage, this, &MainWindow::showRegisterWidget);
    connect(loginWidget, &Login::loginSuccessfully, this, &MainWindow::showInfoWidget);
    connect(registerWidget, &Register::registerSuccessfully, this, &MainWindow::showLoginWidget);

    // Chuyển session id từ widget này sang widget khác
    connect(this, &MainWindow::dispatchSessionId, infoWidget, &Info::storeSessionId);

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

void MainWindow::showInfoWidget(QString sessionId)
{
    emit dispatchSessionId(sessionId);
    ui->stackedWidget->setCurrentIndex(2);
}


