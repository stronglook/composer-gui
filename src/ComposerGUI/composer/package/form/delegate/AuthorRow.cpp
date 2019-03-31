#include <QAbstractItemModel>

#include "AuthorRow.h"
#include "editor/Author.h"

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

QWidget *AuthorRow::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);

    editor::Author *authorEditor = new editor::Author(parent);
    authorEditor->setModal(true);
    authorEditor->setMinimumWidth(500);
    authorEditor->setWindowTitle("Author information");
    authorEditor->setWindowFlags(authorEditor->windowFlags() &
                                 ~Qt::WindowContextHelpButtonHint);

    connect(authorEditor, &QDialog::rejected, [index]
    {
        QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());

        bool isEmptyRow = true;
        for (int i = 0; i < model->columnCount(); i++)
        {
            const QString &columnValue = index.sibling(index.row(), i).data().toString().trimmed();

            if (!columnValue.isEmpty()) {
                isEmptyRow = false;
                break;
            }
        }

        if (isEmptyRow) {
            model->removeRow(model->rowCount() - 1);
        }
    });

    return authorEditor;
}

void AuthorRow::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    editor::Author *authorEditor = static_cast<editor::Author *>(editor);
    authorEditor->setModelIndex(index);
}


} // delegate
} // form
} // package
} // composer
} // composer_gui
