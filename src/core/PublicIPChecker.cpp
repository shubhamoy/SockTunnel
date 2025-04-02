// core/PublicIPChecker.cpp
#include "PublicIPChecker.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkProxy>
#include <QDebug>

PublicIPChecker::PublicIPChecker(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this)), proxyHost("127.0.0.1"), proxyPort(5000) {
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &PublicIPChecker::handleReply);
}

void PublicIPChecker::setSocksProxyAddress(const QString &host, int port) {
    proxyHost = host;
    proxyPort = port;
}

void PublicIPChecker::fetchIP(bool initial) {
    currentInitialFlag = initial;
    QUrl url("https://api.ipify.org");
    QNetworkRequest request(url);

    if (!initial) {
        QNetworkProxy proxy;
        proxy.setType(QNetworkProxy::Socks5Proxy);
        proxy.setHostName(proxyHost);
        proxy.setPort(proxyPort);
        QNetworkProxy::setApplicationProxy(proxy);
    } else {
        QNetworkProxy::setApplicationProxy(QNetworkProxy());
    }

    networkManager->get(request);
}

void PublicIPChecker::handleReply(QNetworkReply *reply) {
    QString ip = QString::fromUtf8(reply->readAll()).trimmed();

    if (reply->error() == QNetworkReply::NoError) {
        emit ipFetched(ip, currentInitialFlag);
        qDebug() << "[DEBUG] Public IP (" << (currentInitialFlag ? "direct" : "tunneled") << "):" << ip;
    } else {
        emit ipFetchFailed(reply->errorString(), currentInitialFlag);
        qWarning() << "[ERROR] Failed to fetch public IP:" << reply->errorString();
    }

    reply->deleteLater();
}

