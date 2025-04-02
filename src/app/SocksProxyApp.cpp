// app/SocksProxyApp.cpp
#include "SocksProxyApp.h"

#include <QMessageBox>
#include <QDebug>

SocksProxyApp::SocksProxyApp(QWidget *parent)
    : QWidget(parent),
      ui(new SocksProxyUI(this)),
      proxyManager(new SshProxyManager(this)),
      ipChecker(new PublicIPChecker(this)),
      proxyConfigurer(new SystemProxyConfigurer) {

    setWindowTitle("SockTunnel");
    setFixedSize(400, 400);

    SshConfigParser parser;
    sshConfig = parser.parse();

    setLayout(ui->layout());
    QComboBox *hostCombo = ui->hostComboBox();
    for (const QString &host : sshConfig.keys()) {
        hostCombo->addItem(host);
    }

    connect(proxyManager, &SshProxyManager::proxyStarted, this, &SocksProxyApp::onProxyStarted);
    connect(proxyManager, &SshProxyManager::proxyFailed, this, &SocksProxyApp::onProxyFailed);
    connect(proxyManager, &SshProxyManager::proxyStopped, this, &SocksProxyApp::onProxyStopped);

    connect(ipChecker, &PublicIPChecker::ipFetched, this, &SocksProxyApp::onIPFetched);
    connect(ipChecker, &PublicIPChecker::ipFetchFailed, this, &SocksProxyApp::onIPFetchFailed);

    connect(ui->connectButton(), &QPushButton::clicked, this, &SocksProxyApp::handleConnectClicked);
    connect(ui->disconnectButton(), &QPushButton::clicked, this, &SocksProxyApp::handleDisconnectClicked);

    updatePublicIP(true);
}

SocksProxyApp::~SocksProxyApp() {
    proxyManager->stopProxy();
    delete proxyConfigurer;
}

void SocksProxyApp::handleConnectClicked() {
    if (ui->hostComboBox()->currentIndex() == 0) {
        QMessageBox::warning(this, "No Host Selected", "Please select a valid host before connecting.");
        return;
    }

    QString host = ui->hostComboBox()->currentText();
    ui->setStatusText("Status: Connecting...");
    ui->setStatusColor("yellow");
    ui->connectButton()->setEnabled(false);

    proxyManager->startProxy(host, socksPort);
}

void SocksProxyApp::handleDisconnectClicked() {
    proxyManager->stopProxy();
}

void SocksProxyApp::onProxyStarted(int port) {
    socksPort = port;
    proxyConfigurer->setSocksProxy(socksPort);
    ipChecker->setSocksProxyAddress(socksHost, socksPort);
    updatePublicIP(false);

    ui->disconnectButton()->setEnabled(true);
    ui->setStatusText(QString("Status: Connected on port %1").arg(socksPort));
    ui->setStatusColor("green");
    QMessageBox::information(this, "Connected", QString("SockTunnel started on port %1").arg(socksPort));
}

void SocksProxyApp::onProxyFailed(const QString &error) {
    QMessageBox::critical(this, "Connection Failed", error);
    ui->connectButton()->setEnabled(true);
    ui->setStatusColor("red");
    ui->setStatusText("Status: Disconnected");
}

void SocksProxyApp::onProxyStopped() {
    proxyConfigurer->unsetSocksProxy();
    ui->connectButton()->setEnabled(true);
    ui->disconnectButton()->setEnabled(false);
    ui->setStatusColor("red");
    ui->setStatusText("Status: Disconnected");
    updatePublicIP(true);
}

void SocksProxyApp::onIPFetched(const QString &ip, bool initial) {
    QString label = initial ? QString("Public IP: %1 (direct)").arg(ip)
                            : QString("Public IP: %1 (tunneled)").arg(ip);
    ui->ipLabel()->setText(label);
}

void SocksProxyApp::onIPFetchFailed(const QString &error, bool initial) {
    Q_UNUSED(error);
    QString label = initial ? "Public IP: Unknown (direct)"
                            : "Public IP: Unknown (tunneled)";
    ui->ipLabel()->setText(label);
}

void SocksProxyApp::updatePublicIP(bool initial) {
    ipChecker->fetchIP(initial);
}