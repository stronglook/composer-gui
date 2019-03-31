#include <QStandardItemModel>

#include "RepositoryRow.h"
#include "editor/Repository.h"

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

QWidget *RepositoryRow::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    editor::Repository *repositoryEditor = new editor::Repository(parent);
    repositoryEditor->setModal(true);
    repositoryEditor->setMinimumWidth(500);
    repositoryEditor->setWindowTitle("Repository information");
    repositoryEditor->setWindowFlags(repositoryEditor->windowFlags() &
                                     ~Qt::WindowContextHelpButtonHint);

    connect(repositoryEditor, &QDialog::rejected, [index]
    {
        QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
        if (index.data().toString().trimmed().isEmpty()) {
            model->removeRow(model->rowCount() - 1);
        }
    });

    return repositoryEditor;
}

void RepositoryRow::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    editor::Repository *repositoryEditor = static_cast<editor::Repository *>(editor);
    repositoryEditor->setModelIndex(index);
}

} // delegate
} // form
} // package
} // composer
} // composer_gui
