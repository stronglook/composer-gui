#include <QMessageBox>

#include "Context.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace controls
{

Context::Context(QAbstractItemView *view) : m_view(view)
{
    m_menu.addAction("Edit", [this]
    {
        edit();
    });

    m_menu.addSeparator();

    m_menu.addAction("Remove", [this]
    {
        remove();
    });
}

void Context::show(const QPoint &pos)
{
    m_menu.exec(m_view->mapToGlobal(pos));
}

void Context::edit()
{
    m_view->edit(m_view->currentIndex());
}

void Context::remove()
{
    const QModelIndex &index = m_view->currentIndex();
    const QModelIndex &parent = index.parent();
    bool hasParent = parent.isValid();

    QAbstractItemModel *model = static_cast<QAbstractItemModel *>(m_view->model());

    QString question = "Do you want remove " + index.sibling(index.row(), 0).data().toString() + '?';
    if (QMessageBox::question(m_view, "Confirm", question) == QMessageBox::StandardButton::Yes) {
        model->removeRow(index.row(), parent);
        if (hasParent && !m_view->currentIndex().parent().isValid()) {
            model->removeRow(index.row());
        }
    }
}


} // controls
} // form
} // package
} // composer
} // composer_gui
