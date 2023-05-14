#include "register.h"
#include "ui_register.h"
#include "QLineEdit"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    ui->passInput->setEchoMode(QLineEdit::Password);

    connect(this, &Register::registerFailed, this, &Register::handleError);
}

Register::~Register()
{
    delete ui;
}

void Register::showEvent(QShowEvent *event)
{
    // Gọi phương thức showEvent() của lớp cơ sở để xử lý sự kiện mặc định
    QWidget::showEvent(event);

    // Xóa nội dung
    ui->userInput->clear();
    ui->passInput->clear();
    ui->message->clear();
}

void Register::handleError(QString msg)
{
    QString newMsg = "<font color='red'>" + msg + "</font>";
    ui->message->setText(newMsg);
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

    manager = new QNetworkAccessManager(this);

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


void Register::on_returnLoginBtn_clicked()
{
    emit changeToLogin();
}

