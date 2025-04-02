// app/SocksProxyApp.h
#pragma once

#include <QWidget>
#include <QMap>
#include <QString>

#include "core/SshProxyManager.h"
#include "core/SshConfigParser.h"
#include "core/PublicIPChecker.h"
#include "core/SystemProxyConfigurer.h"
#include "ui/SocksProxyUI.h"

class SocksProxyApp : public QWidget {
    Q_OBJECT

public:
    explicit SocksProxyApp(QWidget *parent = nullptr);
    void handleDisconnectClicked();
    ~SocksProxyApp();

private slots:
    void handleConnectClicked();
    
    void onProxyStarted(int port);
    void onProxyFailed(const QString &error);
    void onProxyStopped();
    void onIPFetched(const QString &ip, bool initial);
    void onIPFetchFailed(const QString &error, bool initial);

private:
    void updatePublicIP(bool initial);

    SocksProxyUI *ui;
    SshProxyManager *proxyManager;
    SshConfigParser::HostConfigMap sshConfig;
    PublicIPChecker *ipChecker;
    SystemProxyConfigurer *proxyConfigurer;

    QString socksHost = "127.0.0.1";
    int socksPort = 5000;
};