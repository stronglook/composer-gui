#include <QStringList>

#include "DependencyRow.h"
#include "composer/package/form/delegate/editor/Dependency.h"

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

DependencyRow::DependencyRow(const QString &title) : m_title(title){}

QWidget *DependencyRow::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option);
    Q_UNUSED(index);

    editor::Dependency *dependencyEditor = new editor::Dependency(parent);
    dependencyEditor->setModal(true);
    dependencyEditor->setMinimumWidth(500);
    dependencyEditor->setWindowTitle(m_title);
    dependencyEditor->setWindowFlags(dependencyEditor->windowFlags() &
                                     ~Qt::WindowContextHelpButtonHint);

    connect(dependencyEditor, &QDialog::rejected, [index]
    {
        if (index.data().toString().isEmpty()) {
            QAbstractItemModel *model = const_cast<QAbstractItemModel *>(index.model());
            model->removeRow(model->rowCount() - 1);
        }
    });

    return dependencyEditor;
}

void DependencyRow::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    editor::Dependency *dependencyEditor = static_cast<editor::Dependency *>(editor);
    dependencyEditor->setModelIndex(index);
}

} // delegate
} // form
} // package
} // composer
} // composer_gui
