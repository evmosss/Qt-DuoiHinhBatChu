#ifndef REGISTER_H
#define REGISTER_H

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

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();

private:
    Ui::Register *ui;
    void handleError(QString msg);
    QNetworkAccessManager * manager;

signals:
    void registerSuccessfully();
    void registerFailed(QString message);

private slots:
    void on_registerButton_clicked();
};

#endif // REGISTER_H
