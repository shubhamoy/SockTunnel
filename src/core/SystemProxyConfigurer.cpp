#include "SystemProxyConfigurer.h"

#include <QProcess>
#include <QDebug>
#include <QByteArray>

#ifdef Q_OS_LINUX
#include <QProcessEnvironment>
#endif

void SystemProxyConfigurer::setSocksProxy(int port) {
#ifdef Q_OS_MACOS
    QProcess proc;
    proc.start("networksetup", QStringList() << "-listallnetworkservices");
    proc.waitForFinished();
    QString output = proc.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (int i = 1; i < lines.size(); ++i) {
        QString service = lines.at(i).trimmed();
        QProcess::execute("networksetup", {"-setsocksfirewallproxy", service, "127.0.0.1", QString::number(port)});
        QProcess::execute("networksetup", {"-setsocksfirewallproxystate", service, "on"});
        qDebug() << "[DEBUG] macOS proxy set for service:" << service;
    }
#elif defined(Q_OS_LINUX)
    qputenv("ALL_PROXY", QString("socks5h://127.0.0.1:%1").arg(port).toUtf8());
    qDebug() << "[DEBUG] Linux ALL_PROXY environment variable set";
#else
    qDebug() << "[WARNING] System proxy not set: Unsupported OS";
#endif
}

void SystemProxyConfigurer::unsetSocksProxy() {
#ifdef Q_OS_MACOS
    QProcess proc;
    proc.start("networksetup", QStringList() << "-listallnetworkservices");
    proc.waitForFinished();
    QString output = proc.readAllStandardOutput();
    QStringList lines = output.split('\n', Qt::SkipEmptyParts);
    for (int i = 1; i < lines.size(); ++i) {
        QString service = lines.at(i).trimmed();
        QProcess::execute("networksetup", {"-setsocksfirewallproxystate", service, "off"});
        qDebug() << "[DEBUG] macOS proxy removed for service:" << service;
    }
#elif defined(Q_OS_LINUX)
    qunsetenv("ALL_PROXY");
    qDebug() << "[DEBUG] Linux ALL_PROXY environment variable unset";
#endif
}
