
#include "History.h"

History::History()
{

}

void History::createHistory(int winnerId, int loserId)
{
    QJsonObject response;
    QSqlDatabase database = Database::getInstance().getDatabase();
    QSqlQuery query(database);

    query.prepare("INSERT INTO histories(winner_id, loser_id, match_at) values(:winnerId, :loserId, :matchAt)");
    query.bindValue(":winnerId", winnerId);
    query.bindValue(":loserId", loserId);
    query.bindValue(":matchAt", QDateTime::currentDateTimeUtc().toString());
    if (!query.exec()) {
        qInfo() << query.lastError().text();
    };

}

