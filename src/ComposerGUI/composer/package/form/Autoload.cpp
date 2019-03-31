#include <QMessageBox>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QLabel>

#include "Autoload.h"
#include "delegate/AutoloadRow.h"
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

Autoload::Autoload(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *autoloadersCommonLayout = new QVBoxLayout;

    for (const QString &type : {"autoload", "autoload-dev"})
    {
        QHBoxLayout *autoloadersLayout = new QHBoxLayout;
        QTreeView *autoloadersView = createView(type);
        autoloadersLayout->addWidget(autoloadersView);
        autoloadersLayout->addWidget(new controls::FixedTree(autoloadersView));
        autoloadersCommonLayout->addLayout(autoloadersLayout);
    }

    setLayout(autoloadersCommonLayout);
}

void Autoload::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

QStandardItemModel *Autoload::getMappingModel(const QString &name)
{
    return m_models[name];
}

QTreeView *Autoload::createView(const QString &name)
{
    QTreeView *autoloaders = new QTreeView;
    auto delegate = new delegate::AutoloadRow;
    connect(autoloaders, &QTreeView::destroyed, delegate, &delegate::AutoloadRow::deleteLater);

    autoloaders->setItemDelegate(delegate);
    autoloaders->setContextMenuPolicy(Qt::CustomContextMenu);
    autoloaders->setModel(createModel(name));

    connect(autoloaders, &QTreeView::customContextMenuRequested, [autoloaders](const QPoint &pos)
    {
        const QModelIndex &index = autoloaders->currentIndex();
        if (index.isValid() && index.parent().isValid()) {
            controls::Context contextControls(autoloaders);
            contextControls.show(pos);
        }
    });

    return autoloaders;
}

QStandardItemModel *Autoload::createModel(const QString &name)
{
    QStandardItemModel *model = new model::FixedTreeDuplicateAware(this);
    model->setColumnCount(1);
    model->setHeaderData(0, Qt::Horizontal, name);

    m_models[name] = model;

    return model;
}

} // form
} // package
} // composer
} // composer_gui
