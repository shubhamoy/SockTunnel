// ui/CenterAlignDelegate.h
#pragma once

#include <QStyledItemDelegate>

class CenterAlignDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const override;
};

