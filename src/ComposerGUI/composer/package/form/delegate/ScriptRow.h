#pragma once

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

class ScriptRow : public BaseDelegate
{
public:
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
};

} // delegate
} // form
} // package
} // composer
} // composer_gui
