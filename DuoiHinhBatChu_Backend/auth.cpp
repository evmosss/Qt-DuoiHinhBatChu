
#include "auth.h"
#include "QByteArray"
#include "QString"
#include "QHttpServerResponse"
#include "QJsonObject"
#include "QUuid"
#include "QNetworkCookie"
#include "QDateTime"
#include <QSqlRecord>

#define _DATABASE_NAME "SP2"

Auth::Auth()
{

}

QHttpServerResponse Auth::login(QString *username, QString *password)
{
    database = Database::getInstance().getDatabase();
    QSqlQuery query(database);

    query.prepare("SELECT * FROM users WHERE username = :username");
    query.bindValue(":username", *username);

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return QHttpServerResponse(QJsonObject{ {"message", "Something went wrong!"} }, QHttpServerResponder::StatusCode::InternalServerError);
    }

    if (query.next()) {
        QString userId = query.value("id").toString();
        qInfo()  << "[+] User " << userId << " are trying to login!";
        int userIdInteger = query.value("id").toInt();
        QString hashedPassword = query.value("password").toString();
        if (Auth::checkPassword(*password, hashedPassword)) {
            QString sessionToken = QUuid::createUuid().toString().mid(1, 36);
            sessionToken.remove('-');

            // Handle create user_session
            query.clear();
            query.prepare("SELECT * FROM user_sessions WHERE user_id = :userId");
            query.bindValue(":userId", userId);
            if (!query.exec()) {
                qDebug() << "Failed to execute query:" << query.lastError().text();
                return QHttpServerResponse(QJsonObject{ {"message", query.lastError().text() } }, QHttpServerResponder::StatusCode::InternalServerError);
            }
            if (query.next()) {
                bool isLogin = query.value("status").toBool();
                qInfo() << "Login status: " << isLogin << query.value("user_id").toInt();

                if (isLogin) {
                    return QHttpServerResponse(QJsonObject{ {"message", "This account is on login status!" } }, QHttpServerResponder::StatusCode::InternalServerError);
                }
                query.clear();
                query.prepare("UPDATE user_sessions SET session_id = :session, valid_date = :date, status = true WHERE user_id = :id");
                query.bindValue(":id", userId);
                query.bindValue(":session", sessionToken);
                query.bindValue(":date", QDateTime::currentDateTimeUtc().addDays(1).toString());

                if (!query.exec()) {
                    return QHttpServerResponse(QJsonObject{ {"message", query.lastError().text()} }, QHttpServerResponder::StatusCode::InternalServerError);
                }
            }
            else {
                query.clear();
                query.prepare("INSERT INTO user_sessions(user_id, session_id, valid_date) values(:id, :session, :date)");
                query.bindValue(":id", userId);
                query.bindValue(":session", sessionToken);
                query.bindValue(":date", QDateTime::currentDateTimeUtc().addDays(1).toString());

                if (!query.exec()) {
                    return QHttpServerResponse(QJsonObject{ {"message", query.lastError().text()} }, QHttpServerResponder::StatusCode::InternalServerError);
                }
            }

            return QHttpServerResponse(QJsonObject {{"message", "Login Succesffully"}, {"sessionId", sessionToken }, {"userId", userIdInteger}}, QHttpServerResponder::StatusCode::Ok);
        } else {
            return QHttpServerResponse(QJsonObject {{"message", "Wrong password"}}, QHttpServerResponder::StatusCode::BadRequest);
        }
    }
    else {
        return QHttpServerResponse(QJsonObject {{"message", "User does not exist"}}, QHttpServerResponder::StatusCode::BadRequest);
    }
}

QHttpServerResponse Auth::registerAccount(QString *username, QString *password)
{
    database = Database::getInstance().getDatabase();
    QSqlQuery query(database);

    QString hashedPassword = hashPassword(*password);
    query.prepare("INSERT INTO users(username, password) values(:username,:password)");
    query.bindValue(":username", *username);
    query.bindValue(":password", hashedPassword);
    if (!query.exec()) {
        return QHttpServerResponse(QJsonObject {{"message", query.lastError().text()}}, QHttpServerResponder::StatusCode::BadRequest);
    };

    return QHttpServerResponse(QJsonObject {{"message", "Register Successfully"}}, QHttpServerResponder::StatusCode::Ok);
}

bool Auth::checkPassword(const QString &password, const QString &hashedPassword)
{
    return Auth::hashPassword(password) == hashedPassword;
}

QString Auth::hashPassword(const QString &password)
{
    QByteArray hash = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

