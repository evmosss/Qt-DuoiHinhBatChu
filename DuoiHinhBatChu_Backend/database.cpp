// Database.cpp
#include "database.h"

Database& Database::getInstance()
{
    static Database instance;
    return instance;
}

QSqlDatabase& Database::getDatabase()
{
    return m_database;
}

Database::Database(QObject *parent)
    : QObject(parent)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE", _DATABASE_NAME);
    m_database.setDatabaseName("D:/Developer/Qt/DuoiHinhBatChu/altp.db");

    if(!m_database.open()) {
        qDebug() << "Error: Unable to open database..";
    }

    if (m_database.open()){
        qInfo() << "Successfully: Connecting to database..";
    }

}
