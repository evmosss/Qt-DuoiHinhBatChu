#include "register.h"
#include "ui_register.h"
#include "QLineEdit"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    ui->passInput->setEchoMode(QLineEdit::Password);

    connect(this, Register::registerFailed, this, Register::handleError);
}

Register::~Register()
{
    delete ui;
}

void Register::handleError(QString msg)
{
    ui->message->setText(msg);
}

void Register::on_registerButton_clicked()
{
    QString username = ui->userInput->text();
    QString password = ui->passInput->text();

    QJsonObject json;
    json.insert("username", username);
    json.insert("password", password);
    QJsonDocument jsonData(json);
    QByteArray data = jsonData.toJson();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);;

    QUrl url(API_URL + "auth/register");
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
            emit registerSuccessfully();
        } else {
            emit registerFailed(reply->errorString());
        }

        reply->deleteLater();
    });
}

