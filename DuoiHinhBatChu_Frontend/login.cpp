#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(this, Login::loginFailed, this, Login::handleError);
}

Login::~Login()
{
    delete ui;
}

void Login::handleError(QString msg)
{
    ui->message->setText(msg);
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

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);;

    QUrl url(API_URL + "auth/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    manager->post(request, data);

    connect(manager, &QNetworkAccessManager::finished, [&](QNetworkReply *reply) {
        qInfo() << "Run here";
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray response = reply->readAll();
            QJsonDocument jsonDoc(QJsonDocument::fromJson(response));
            QJsonObject jsonObj = jsonDoc.object();
            // Xử lý dữ liệu JSON ở đây
            QString sessionId = jsonObj.value("sessionId").toString();
            qInfo() << this->parent();
            emit loginSuccessfully(sessionId);
        } else {
            emit loginFailed(reply->errorString());
        }

        reply->deleteLater();
    });

}
