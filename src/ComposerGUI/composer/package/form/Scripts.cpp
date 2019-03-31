#include <QHBoxLayout>
#include <QPushButton>
#include <QTreeView>

#include "Scripts.h"
#include "delegate/ScriptRow.h"
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

Scripts::Scripts(QWidget *parent)
    : QWidget(parent),
      m_model(new model::FixedTreeDuplicateAware(this))
{
    m_model->setColumnCount(1);
    m_model->setHeaderData(0, Qt::Horizontal, "Events");

    QTreeView *scripts = new QTreeView;
    auto delegate = new delegate::ScriptRow;
    connect(scripts, &QTreeView::destroyed, delegate, &delegate::ScriptRow::deleteLater);

    scripts->setItemDelegate(delegate);
    scripts->setContextMenuPolicy(Qt::CustomContextMenu);
    scripts->setModel(m_model);

    connect(scripts, &QTreeView::customContextMenuRequested, [scripts](const QPoint &pos)
    {
        const QModelIndex &index = scripts->currentIndex();
        if (index.isValid() && index.parent().isValid()) {
            controls::Context contextControls(scripts);
            contextControls.show(pos);
        }
    });

    QHBoxLayout *scriptsLayout = new QHBoxLayout;
    scriptsLayout->addWidget(scripts);
    scriptsLayout->addWidget(new controls::FixedTree(scripts));

    setLayout(scriptsLayout);
}

void Scripts::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

QStandardItemModel *Scripts::getMappingModel()
{
    return m_model;
}

} // form
} // package
} // composer
} // composer_gui
