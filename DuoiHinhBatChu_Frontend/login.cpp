#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(this, &Login::loginFailed, this, &Login::handleError);
}

Login::~Login()
{
    delete ui;
}

void Login::showEvent(QShowEvent *event)
{
    // Gọi phương thức showEvent() của lớp cơ sở để xử lý sự kiện mặc định
    QWidget::showEvent(event);

    // Xóa nội dung
    ui->userInput->clear();
    ui->passInput->clear();
    ui->message->clear();
}

void Login::handleError(QString msg)
{
    QString newMsg = "<font color='red'>" + msg + "</font>";
    ui->message->setText(newMsg);
}

void Login::on_registerButton_clicked()
{
    emit changeToRegisterPage();
}

void Login::on_loginButton_clicked()
{
    QString username = ui->userInput->text();
    QString password = ui->passInput->text();

    QJsonObject json;
    json.insert("username", username);
    json.insert("password", password);
    QJsonDocument jsonData(json);
    QByteArray data = jsonData.toJson();

    manager = new QNetworkAccessManager(this);

    QUrl url(API_URL + "auth/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager->post(request, data);

    connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc(QJsonDocument::fromJson(response));
            QJsonObject jsonObj = jsonDoc.object();
            // Xử lý dữ liệu JSON ở đây
            QString sessionId = jsonObj.value("sessionId").toString();
            int userId = jsonObj.value("userId").toInt();

            if (userId == 0) {
                emit loginFailed("Something went wrong");
            }

            emit loginSuccessfully(sessionId, userId);
        } else {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc(QJsonDocument::fromJson(response));
            QJsonObject jsonObj = jsonDoc.object();
            emit loginFailed(jsonObj.value("message").toString());
        }

        reply->deleteLater();
    });

}
