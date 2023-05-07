#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    static NetworkManager& getInstance();
    QNetworkAccessManager* network() const { m_manager->clearConnectionCache(); m_manager->clearAccessCache(); return m_manager; }

private:
    explicit NetworkManager(QObject* parent = nullptr);
    NetworkManager(const NetworkManager&) = delete;
    NetworkManager& operator=(const NetworkManager&) = delete;

    QNetworkAccessManager* m_manager;
};

#endif // NETWORKMANAGER_H
