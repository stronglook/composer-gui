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

void BaseDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // stub

    Q_UNUSED(editor);
    Q_UNUSED(model);
    Q_UNUSED(index);
}

void BaseDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // stub

    Q_UNUSED(editor);
    Q_UNUSED(option);
    Q_UNUSED(index);
}

} // delegate
} // form
} // package
} // composer
} // composer_gui
