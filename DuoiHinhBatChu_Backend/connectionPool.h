#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QSqlRecord>
#include <QMutex>

// Currently, not touch this file yet
class ConnectionPool
{
public:
    static ConnectionPool& instance()
    {
        static ConnectionPool instance;
        return instance;
    }

    QSqlDatabase getConnection()
    {
        QMutexLocker locker(&m_mutex);

        if (m_connectionCount < m_maxConnections)
        {
            QSqlDatabase db = createConnection();
            m_connectionCount++;
            return db;
        }

        for (auto& db : m_connections)
        {
            if (db.isValid() == true && m_busyConnections.contains(db) == false)
            {
                m_busyConnections.insert(db);
                return db;
            }
        }

        return QSqlDatabase();
    }

    void releaseConnection(QSqlDatabase db)
    {
        QMutexLocker locker(&m_mutex);
        if (m_connections.contains(db) == true && m_busyConnections.contains(db) == true)
        {
            m_busyConnections.remove(db);
        }
    }

private:
    ConnectionPool()
        : m_connectionCount(0)
        , m_maxConnections(10)
    {
        // Initialize your database settings here
        QSqlDatabase database = QSqlDatabase::addDatabase("QSQLITE", "CPool");
        database.setDatabaseName("./altp.db");
    }

    QSqlDatabase createConnection()
    {
        QSqlDatabase db = QSqlDatabase::cloneDatabase(QSqlDatabase::database("default"), QString("connection%1").arg(m_connectionCount + 1));
        if (db.open())
        {
            m_connections.insert(db);
        }
        else
        {
            qCritical() << "Failed to create a database connection: " << db.lastError().text();
        }
        return db;
    }

    QSet<QSqlDatabase> m_connections;
    QSet<QSqlDatabase> m_busyConnections;
    QMutex m_mutex;
    int m_connectionCount;
    int m_maxConnections;
};

bool operator==(const QSqlDatabase& db1, const QSqlDatabase& db2)
{
    return db1.connectionName() == db2.connectionName();
}
