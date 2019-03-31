#include <QStandardItemModel>

#include "ScriptRow.h"
#include "editor/Script.h"

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

QWidget *ScriptRow::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    editor::Script *scriptEditor = new editor::Script(parent);
    scriptEditor->setModal(true);
    scriptEditor->setMinimumWidth(500);
    scriptEditor->setWindowTitle("Script information");
    scriptEditor->setWindowFlags(scriptEditor->windowFlags() &
                                 ~Qt::WindowContextHelpButtonHint);

    connect(scriptEditor, &QDialog::rejected, [index]
    {
        QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
        if (index.data().toString().trimmed().isEmpty()) {
            model->removeRow(model->rowCount() - 1);
        }
    });

    return scriptEditor;
}

void ScriptRow::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    editor::Script *scriptEditor = static_cast<editor::Script *>(editor);
    scriptEditor->setModelIndex(index);
}

} // delegate
} // form
} // package
} // composer
} // composer_gui
