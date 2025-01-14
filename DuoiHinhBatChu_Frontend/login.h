#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
// For database connecting
#include <QDebug>
#include <QCryptographicHash>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUrl>
#include "config.h"
#include <QJsonObject>
#include <QJsonDocument>
#include "networkmanager.h"

extern const QString API_URL;

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::Login *ui;
    void handleError(QString msg);
    QNetworkAccessManager *manager;

signals:
    void changeToRegisterPage();
    void loginSuccessfully(QString sessionId, int userId);
    void loginFailed(QString message);

private slots:
    void on_registerButton_clicked();
    void on_loginButton_clicked();
};

#endif // LOGIN_H
