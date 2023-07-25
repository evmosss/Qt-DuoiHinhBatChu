#include "rank.h"

QHttpServerResponse Rank::getAllRanks(int page, int limit)
{
    QJsonObject response;
    QSqlQuery query(database);

    query.prepare("SELECT u.id, u.username, u.point, COUNT(h.id) AS total_games, SUM(CASE WHEN h.winner_id = u.id THEN 1 ELSE 0 END) AS win_count, 100 * SUM(CASE WHEN h.winner_id = u.id THEN 1 ELSE 0 END) / COUNT(h.id) AS win_ratio FROM users AS u LEFT JOIN histories AS h ON h.winner_id = u.id OR h.loser_id = u.id GROUP BY u.id ORDER BY u.point DESC LIMIT :limit OFFSET :offset");
    query.bindValue(":limit", limit);
    query.bindValue(":offset", (page - 1) * limit);

    if (!query.exec()) {
        return QHttpServerResponse(QJsonObject{ {"message", query.lastError().text() } }, QHttpServerResponder::StatusCode::InternalServerError);
    }

    QJsonObject res;
    QJsonArray data;

    while (query.next()) {
        QJsonObject row;
        row["id"] = query.value(0).toInt();
        row["id_username"] = query.value(1).toString();
        row["point"] = query.value(2).toInt();
//        row["rank"] = getRoomDifficultyFromPoint(static_cast<RoomDifficulty>(query.value(2).toInt()));
        if(query.value(2).toInt() <= 10) {
            row["rank"] = "Easy";
        } else if (query.value(2).toInt() > 10 && query.value(2).toInt() <= 20) {
            row["rank"] = "Medium";
        } else if (query.value(2).toInt() > 20 && query.value(2).toInt() <= 30) {
            row["rank"] = "Hard";
        }
        else {
            row["rank"] = "Advanced";
        }
        row["total_games"] = query.value(3).toInt();
        row["win_count"] = query.value(4).toInt();
        row["win_ratio"] = query.value(5).toDouble();

        data.append(row);
    }

    res["data"] = data;
    res["page"] = page;
    res["limit"] = limit;

    return QHttpServerResponse(QJsonObject{ {"message", "Get all rank successfully" }, {"data", res} }, QHttpServerResponder::StatusCode::Ok);
}

Rank::Rank()
{
    database = Database::getInstance().getDatabase();
}

