// core/PublicIPChecker.h
#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class PublicIPChecker : public QObject {
    Q_OBJECT

public:
    explicit PublicIPChecker(QObject *parent = nullptr);
    void fetchIP(bool initial);
    void setSocksProxyAddress(const QString &host, int port);

signals:
    void ipFetched(const QString &ip, bool initial);
    void ipFetchFailed(const QString &error, bool initial);

private slots:
    void handleReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QString proxyHost;
    int proxyPort;
    bool currentInitialFlag;
};
