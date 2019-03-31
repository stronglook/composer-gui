#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeView>

#include "Repositories.h"
#include "delegate/RepositoryRow.h"
#include "composer/package/form/controls/Context.h"
#include "composer/package/form/controls/FixedTree.h"
#include "composer/package/form/model/FixedTreeDuplicateAware.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

Repositories::Repositories(QWidget *parent)
    : QWidget(parent),
      m_model(new model::FixedTreeDuplicateAware(this))
{
    m_model->setColumnCount(1);
    m_model->setHeaderData(0, Qt::Horizontal, "Repositories");

    QTreeView *repositories = new QTreeView;
    auto delegate = new delegate::RepositoryRow;
    connect(repositories, &QTreeView::destroyed, delegate, &delegate::RepositoryRow::deleteLater);

    repositories->setItemDelegate(delegate);
    repositories->setContextMenuPolicy(Qt::CustomContextMenu);
    repositories->setModel(m_model);

    connect(repositories, &QTreeView::customContextMenuRequested, [repositories](const QPoint &pos)
    {
        const QModelIndex &index = repositories->currentIndex();
        if (index.isValid() && index.parent().isValid()) {
            controls::Context contextControls(repositories);
            contextControls.show(pos);
        }
    });

    QHBoxLayout *repositoriesLayout = new QHBoxLayout;
    repositoriesLayout->addWidget(repositories);
    repositoriesLayout->addWidget(new controls::FixedTree(repositories));

    setLayout(repositoriesLayout);
}

void Repositories::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

QStandardItemModel *Repositories::getMappingModel()
{
    return m_model;
}

} // form
} // package
} // composer
} // composer_gui
