#include <QHBoxLayout>
#include <QHeaderView>
#include <QPoint>

#include "Authors.h"
#include "delegate/AuthorRow.h"
#include "controls/Standard.h"
#include "controls/Context.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

Authors::Authors(QWidget *parent) : QWidget(parent)
{
    QTableView *table = createTableView();
    table->setModel(createTableModel());

    QHBoxLayout *authorsLayout = new QHBoxLayout;
    authorsLayout->addWidget(table);
    authorsLayout->addWidget(new controls::Standard(table, this));

    setLayout(authorsLayout);

    connect(table, &QTableView::customContextMenuRequested, [table](const QPoint &pos)
    {
        if (table->currentIndex().isValid()) {
            controls::Context contextControls(table);
            contextControls.show(pos);
        }
    });
}

void Authors::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

QStandardItemModel *Authors::getMappingModel()
{
    return m_model;
}

QTableView *Authors::createTableView()
{
    QTableView *table = new QTableView;

    QHeaderView *header = table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);
    header->setHighlightSections(false);

    table->setContextMenuPolicy(Qt::CustomContextMenu);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    auto delegate = new delegate::AuthorRow;
    connect(table, &QTableView::destroyed, delegate, &delegate::AuthorRow::deleteLater);
    table->setItemDelegate(delegate);

    return table;
}

QStandardItemModel *Authors::createTableModel()
{
    m_model = new QStandardItemModel(0, 4, this);
    m_model->setHeaderData(0, Qt::Horizontal, "Name");
    m_model->setHeaderData(1, Qt::Horizontal, "Role");
    m_model->setHeaderData(2, Qt::Horizontal, "Email");
    m_model->setHeaderData(3, Qt::Horizontal, "Homepage");

    return m_model;
}

} // form
} // package
} // composer
} // composer_gui
