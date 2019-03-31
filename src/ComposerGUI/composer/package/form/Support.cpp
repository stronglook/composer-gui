#include <QStringList>
#include <QLineEdit>

#include "Support.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

Support::Support(QWidget *parent)
    : QWidget(parent),
      m_mapper(new QDataWidgetMapper(this))
{
    QStandardItemModel *model = new QStandardItemModel(8, 1, this);
    m_mapper->setModel(model);
    m_mapper->setOrientation(Qt::Vertical);
    m_mapper->toFirst();

    connect(model, &QStandardItemModel::itemChanged, m_mapper, &QDataWidgetMapper::toFirst);

    setLayout(createFormLayout());
}

void Support::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

QStandardItemModel *Support::getMappingModel()
{
    return  static_cast<QStandardItemModel *>(m_mapper->model());
}

QFormLayout *Support::createFormLayout()
{
    QStringList labels{"Email:", "Issues:", "Forum:", "Wiki:", "Irc:", "Source:", "Docs:", "Chat:"};
    QFormLayout *formLayout = new QFormLayout;

    for (int i = 0; i < labels.count(); i++)
    {
        QLineEdit *lineEdit = new QLineEdit;
        m_mapper->addMapping(lineEdit, i);

        formLayout->addRow(labels[i], lineEdit);
    }

    return formLayout;
}

} // form
} // package
} // composer
} // composer_gui
