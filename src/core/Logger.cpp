// Logger.cpp
#include "Logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMutex>

static QFile logFile;
static QMutex logMutex;

void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    Q_UNUSED(context);

    QMutexLocker locker(&logMutex);

    if (!logFile.isOpen()) return;

    QTextStream stream(&logFile);
    QString level;

    switch (type) {
        case QtDebugMsg:    level = "DEBUG"; break;
        case QtInfoMsg:     level = "INFO"; break;
        case QtWarningMsg:  level = "WARN"; break;
        case QtCriticalMsg: level = "ERROR"; break;
        case QtFatalMsg:    level = "FATAL"; break;
    }

    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    stream << "[" << timestamp << "] [" << level << "] " << msg << "\n";
    stream.flush();

    if (type == QtFatalMsg) abort();
}

void Logger::init(const QString &logFilePath) {
    logFile.setFileName(logFilePath);
    logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
    qInstallMessageHandler(messageHandler);
}