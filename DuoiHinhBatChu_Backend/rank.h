#ifndef RANK_H
#define RANK_H

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QObject>
#include "QHttpServerResponse"
#include "database.h"
#include <QJsonArray>

class Rank
{
public:
    QHttpServerResponse getAllRanks(int page, int limit);
    QSqlDatabase database;
    Rank();
};

#endif // RANK_H
