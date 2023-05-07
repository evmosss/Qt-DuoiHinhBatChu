#include "networkmanager.h"

NetworkManager& NetworkManager::getInstance()
{
    static NetworkManager instance;
    return instance;
}

NetworkManager::NetworkManager(QObject* parent)
    : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
}
