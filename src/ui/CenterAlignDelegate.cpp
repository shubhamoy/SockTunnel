// ui/CenterAlignDelegate.cpp
#include "CenterAlignDelegate.h"

#include <QStyleOptionViewItem>

void CenterAlignDelegate::initStyleOption(QStyleOptionViewItem *option, const QModelIndex &index) const {
    QStyledItemDelegate::initStyleOption(option, index);
    option->displayAlignment = Qt::AlignCenter;
}