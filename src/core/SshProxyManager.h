// core/SshProxyManager.h
#pragma once

#include <QObject>
#include <QProcess>
#include <QString>

class SshProxyManager : public QObject {
    Q_OBJECT

public:
    explicit SshProxyManager(QObject *parent = nullptr);
    ~SshProxyManager();

    bool startProxy(const QString &host, int &allocatedPort);
    void stopProxy();
    bool isRunning() const;
    int proxyPort() const;

signals:
    void proxyStarted(int port);
    void proxyFailed(const QString &error);
    void proxyStopped();

private:
    bool isPortAvailable(int port) const;
    QProcess *sshProcess;
    int socksPort;
};

