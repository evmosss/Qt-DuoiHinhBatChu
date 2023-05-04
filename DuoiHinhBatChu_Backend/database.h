#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QDebug>

#define _DATABASE_NAME "SP2"

class Database : public QObject
{
    Q_OBJECT
public:
    static Database& getInstance();

    QSqlDatabase& getDatabase();

private:
    explicit Database(QObject *parent = nullptr);
    Database(const Database&) = delete;
    Database& operator=(const Database&) = delete;

    QSqlDatabase m_database;
};

#endif // DATABASE_H


