// ui/SocksProxyUI.h
#pragma once

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QSvgWidget>

class SocksProxyUI : public QWidget {
    Q_OBJECT

public:
    explicit SocksProxyUI(QWidget *parent = nullptr);

    QComboBox* hostComboBox() const;
    QLabel* statusLabel() const;
    QLabel* ipLabel() const;
    QLabel* statusCircle() const;
    QPushButton* connectButton() const;
    QPushButton* disconnectButton() const;

    void setStatusText(const QString &text);
    void setStatusColor(const QString &color);

private:
    QSvgWidget *logo;
    QComboBox *hostCombo;
    QLabel *statusCircleWidget;
    QLabel *statusLabelWidget;
    QLabel *ipLabelWidget;
    QPushButton *connectBtn;
    QPushButton *disconnectBtn;
};
