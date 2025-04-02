// core/SshConfigParser.h
#pragma once

#include <QString>
#include <QMap>

class SshConfigParser {
public:
    using HostConfig = QMap<QString, QString>;
    using HostConfigMap = QMap<QString, HostConfig>;

    QMap<QString, HostConfig> parse(const QString &configPath = QString());
};
