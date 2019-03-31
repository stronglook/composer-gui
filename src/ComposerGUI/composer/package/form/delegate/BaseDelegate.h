#pragma once

#include <QStyledItemDelegate>
#include <QStyleOptionViewItem>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QWidget>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace delegate
{

class BaseDelegate : public QStyledItemDelegate
{
public:
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

} // delegate
} // form
} // package
} // composer
} // composer_gui
