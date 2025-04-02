// core/SshProxyManager.cpp
#include "SshProxyManager.h"

#include <QTcpServer>
#include <QDebug>
#include <QTimer>

SshProxyManager::SshProxyManager(QObject *parent)
    : QObject(parent), sshProcess(nullptr), socksPort(5000) {}

SshProxyManager::~SshProxyManager() {
    stopProxy();
}

bool SshProxyManager::startProxy(const QString &host, int &allocatedPort) {
    if (sshProcess) {
        qWarning() << "[WARN] Proxy is already running.";
        return false;
    }

    // Find an available port
    while (!isPortAvailable(socksPort)) {
        ++socksPort;
    }
    allocatedPort = socksPort;

    sshProcess = new QProcess(this);
    QStringList args;
    args << "-D" << QString("localhost:%1").arg(socksPort)
         << "-N" << host;

    sshProcess->setProcessChannelMode(QProcess::MergedChannels);
    sshProcess->start("ssh", args);

    // Wait a short while to confirm process is running
    QTimer::singleShot(1000, this, [=]() {
        if (sshProcess->state() == QProcess::Running) {
            qDebug() << "[INFO] Establishing SSH Connection to" << host;
            emit proxyStarted(socksPort);
        } else {
            emit proxyFailed("SSH process failed to start.");
            stopProxy();
        }
    });

    return true;
}

void SshProxyManager::stopProxy() {
    if (sshProcess) {
        qDebug() << "[INFO] Stopping SSH proxy on port" << socksPort;
        sshProcess->terminate();
        sshProcess->waitForFinished(3000);
        delete sshProcess;
        sshProcess = nullptr;
        emit proxyStopped();
    }
}

bool SshProxyManager::isRunning() const {
    return sshProcess && sshProcess->state() == QProcess::Running;
}

int SshProxyManager::proxyPort() const {
    return socksPort;
}

bool SshProxyManager::isPortAvailable(int port) const {
    QTcpServer server;
    bool available = server.listen(QHostAddress::LocalHost, port);
    if (available) server.close();
    return available;
}
