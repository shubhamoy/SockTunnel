// core/SshConfigParser.cpp
#include "SshConfigParser.h"

#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

QMap<QString, SshConfigParser::HostConfig> SshConfigParser::parse(const QString &configPath) {
    QMap<QString, HostConfig> configMap;
    QString path = configPath.isEmpty() ? QDir::homePath() + "/.ssh/config" : configPath;
    QFile file(path);

    if (!file.exists()) {
        qWarning() << "[WARN] SSH config not found at" << path;
        return configMap;
    }

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "[ERROR] Failed to open SSH config file:" << file.errorString();
        return configMap;
    }

    QTextStream in(&file);
    QString currentHost;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith("#")) continue;

        if (line.toLower().startsWith("host ")) {
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (parts.size() >= 2) {
                currentHost = parts[1];
                configMap[currentHost] = HostConfig();
            }
        } else if (!currentHost.isEmpty()) {
            QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            if (parts.size() >= 2)
                configMap[currentHost][parts[0].toLower()] = parts[1];
        }
    }

    file.close();
    return configMap;
}
