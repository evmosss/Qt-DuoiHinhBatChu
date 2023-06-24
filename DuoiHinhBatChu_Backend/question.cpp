
#include "question.h"


#define _DATABASE_NAME "SP2"

QJsonObject Question::getRandomQuestion()
{
    QJsonObject response;
    QSqlDatabase database = Database::getInstance().getDatabase();
    QSqlQuery query(database);

    query.prepare("SELECT * FROM questions WHERE id = (SELECT id FROM questions ORDER BY random() LIMIT 1);");
    if (!query.exec()) {
        qInfo() << query.lastError().text();
    }
    if (query.next()) {
        response["id"] = query.value("id").toInt();
        response["image_url"] = query.value("image_url").toString();
        response["answer"] = query.value("answer").toString();
        response["level"] = query.value("level").toInt();
    }
    return response;
}

QHttpServerResponse Question::nextQuestion(int userId, QMap<QString, QJsonObject> *roomData, QString *roomId, QMap<int, QString> *userToRoomId)
{

}

Question::Question()
{

}

