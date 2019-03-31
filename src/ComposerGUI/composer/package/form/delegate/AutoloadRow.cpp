#include "AutoloadRow.h"
#include "editor/Autoloader.h"

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

QWidget *AutoloadRow::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    editor::Autoloader *autoloaderEditor = new editor::Autoloader(parent);
    autoloaderEditor->setModal(true);
    autoloaderEditor->setMinimumWidth(500);
    autoloaderEditor->setWindowTitle("Autoload information");
    autoloaderEditor->setWindowFlags(autoloaderEditor->windowFlags() &
                                     ~Qt::WindowContextHelpButtonHint);

    connect(autoloaderEditor, &QDialog::rejected, [index]
    {
        QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
        if (index.data().toString().trimmed().isEmpty()) {
            model->removeRow(model->rowCount() - 1);
        }
    });

    return autoloaderEditor;
}

void AutoloadRow::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    editor::Autoloader *autoloaderEditor = static_cast<editor::Autoloader *>(editor);
    autoloaderEditor->setModelIndex(index);
}

} // delegate
} // form
} // package
} // composer
} // composer_gui
