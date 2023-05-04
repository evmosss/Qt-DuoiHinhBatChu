
#ifndef QUESTION_H
#define QUESTION_H
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QObject>
#include "database.h"


class Question
{
public:
    QHttpServerResponse nextQuestion(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> * userToRoomId);
    Question();
    static QJsonObject getRandomQuestion();
    QSqlDatabase database;

private:
};

#endif // QUESTION_H
