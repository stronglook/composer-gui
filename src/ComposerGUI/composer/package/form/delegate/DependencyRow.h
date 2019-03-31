#pragma once

#include <QString>

#include "BaseDelegate.h"

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

class DependencyRow : public BaseDelegate
{
public:
    DependencyRow(const QString &title);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;

private:
    QString m_title;
};

} // delegate
} // form
} // package
} // composer
} // composer_gui
