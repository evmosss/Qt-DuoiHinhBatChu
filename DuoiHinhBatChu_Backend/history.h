
#ifndef HISTORY_H
#define HISTORY_H


#include <QObject>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QHttpServerRequest>
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QObject>
#include "database.h"

class History
{
public:
    History();
    static void createHistory(int winnerId, int loserId);
};

#endif // HISTORY_H
