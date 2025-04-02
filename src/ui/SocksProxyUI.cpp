// ui/SocksProxyUI.cpp
#include "SocksProxyUI.h"
#include "CenterAlignDelegate.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSvgWidget>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>

SocksProxyUI::SocksProxyUI(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    logo = new QSvgWidget(":/logo.svg");
    logo->setFixedSize(256, 128);
    layout->addWidget(logo, 0, Qt::AlignCenter);

    hostCombo = new QComboBox();
    hostCombo->addItem("Select a host from ~/.ssh/config");
    hostCombo->setItemDelegate(new CenterAlignDelegate(hostCombo));
    layout->addWidget(hostCombo);

    QHBoxLayout *statusLayout = new QHBoxLayout;
    statusCircleWidget = new QLabel;
    statusCircleWidget->setFixedSize(15, 15);
    statusLabelWidget = new QLabel("Status: Disconnected");
    statusLabelWidget->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    statusLayout->addWidget(statusCircleWidget);
    statusLayout->addWidget(statusLabelWidget);
    statusLayout->setAlignment(Qt::AlignCenter);
    layout->addLayout(statusLayout);

    ipLabelWidget = new QLabel("Public IP: Unknown");
    ipLabelWidget->setAlignment(Qt::AlignCenter);
    layout->addWidget(ipLabelWidget);

    connectBtn = new QPushButton("Connect");
    disconnectBtn = new QPushButton("Disconnect");
    disconnectBtn->setEnabled(false);
    layout->addWidget(connectBtn);
    layout->addWidget(disconnectBtn);
}

QComboBox* SocksProxyUI::hostComboBox() const { return hostCombo; }
QLabel* SocksProxyUI::statusLabel() const { return statusLabelWidget; }
QLabel* SocksProxyUI::ipLabel() const { return ipLabelWidget; }
QLabel* SocksProxyUI::statusCircle() const { return statusCircleWidget; }
QPushButton* SocksProxyUI::connectButton() const { return connectBtn; }
QPushButton* SocksProxyUI::disconnectButton() const { return disconnectBtn; }

void SocksProxyUI::setStatusText(const QString &text) {
    statusLabelWidget->setText(text);
}

void SocksProxyUI::setStatusColor(const QString &color) {
    statusCircleWidget->setStyleSheet(QString("background-color: %1; border-radius: 7px; border: 1px solid black;").arg(color));
}