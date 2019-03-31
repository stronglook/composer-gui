#include <QStandardItemModel>
#include <QStandardItem>
#include <QTreeView>
#include <QMessageBox>

#include "FixedTree.h"

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

FixedTree::FixedTree(QAbstractItemView *view, QWidget *parent) : Standard(view, parent)
{

}

void FixedTree::add()
{
    QStandardItemModel *model = static_cast<QStandardItemModel *>(m_view->model());
    int row = m_view->model()->rowCount();

    QStandardItem *root = new QStandardItem;
    root->setFlags(root->flags() & ~Qt::ItemIsEditable);

    QStandardItem *child = new QStandardItem;

    root->setChild(0, 0, child);

    model->setItem(row, root);

    QModelIndex index = model->index(row, 0);
    m_view->setCurrentIndex(index);
    static_cast<QTreeView *>(m_view)->expand(index);
    m_view->edit(child->index());
}

void FixedTree::edit()
{
    const QModelIndex &index = m_view->currentIndex();

    if (!index.parent().isValid()) {
        return;
    }

    m_view->edit(index);
}

void FixedTree::remove()
{
    const QModelIndex &index = m_view->currentIndex();

    if (!index.parent().isValid()) {
        return;
    }

    QAbstractItemModel *model = static_cast<QAbstractItemModel *>(m_view->model());

    QString question = "Do you want remove " + index.data().toString() + '?';
    if (QMessageBox::question(this, "Confirm", question) == QMessageBox::Yes) {
        model->removeRow(index.row(), index.parent());
        if (!m_view->currentIndex().parent().isValid()) {
            model->removeRow(index.row());
        }
    }
}

} // controls
} // form
} // package
} // composer
} // composer_gui
