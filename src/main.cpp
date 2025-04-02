#include <QApplication>
#include <QIcon>
#include <QFile>

#include "app/SocksProxyApp.h"
#include "core/Logger.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Logger::init("/tmp/socktunnel.log");

    app.setWindowIcon(QIcon(":/icon.icns"));
    QFile styleFile(":/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        app.setStyleSheet(QString::fromUtf8(styleFile.readAll()));
    }

    SocksProxyApp window;
    window.show();

    QObject::connect(&app, &QApplication::aboutToQuit, &window, &SocksProxyApp::handleDisconnectClicked);

    return app.exec();
}

