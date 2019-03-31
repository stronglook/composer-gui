#include <QVBoxLayout>
#include <QHBoxLayout>

#include "General.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

General::General(QWidget *parent)
    : QWidget(parent),
      m_mapper(new QDataWidgetMapper(this))
{
    QStandardItemModel *model = new QStandardItemModel(this);
    model->setRowCount(11);
    model->setColumnCount(1);

    m_mapper->setModel(model);
    m_mapper->setOrientation(Qt::Vertical);
    m_mapper->toFirst();

    QHBoxLayout *forms = new QHBoxLayout;
    forms->addLayout(createLeftFormLayout());
    forms->addLayout(createRightFormLayout());

    m_mapper->submit();
    connect(model, &QStandardItemModel::itemChanged, m_mapper, &QDataWidgetMapper::toFirst);

    QVBoxLayout *generalLayout = new QVBoxLayout;

    generalLayout->addLayout(forms);
    generalLayout->addLayout(createDescriptionFormLayout());

    setLayout(generalLayout);
}

void General::accept(visitor::VisitorInterface &visitor)
{
    visitor.visit(*this);
}

QStandardItemModel *General::getMappingModel()
{
    return static_cast<QStandardItemModel *>(m_mapper->model());
}

QFormLayout *General::createLeftFormLayout()
{
    QLineEdit *name               = new QLineEdit;
    QLineEdit *version            = new QLineEdit;
    QComboBox *type               = new QComboBox;
    QComboBox *minimumStability   = new QComboBox;
    QComboBox *preferStable       = new QComboBox;

    type->addItems({"-", "library", "project", "metapackage", "composer-plugin"});
    minimumStability->addItems({"-", "dev", "alpha", "beta", "RC", "stable"});
    preferStable->addItems({"-", "true", "false"});

    m_mapper->addMapping(name, 0);
    m_mapper->addMapping(version, 1);
    m_mapper->addMapping(type, 2);
    m_mapper->addMapping(minimumStability, 3);
    m_mapper->addMapping(preferStable, 4);

    QFormLayout *form = new QFormLayout;

    form->addRow("Name:", name);
    form->addRow("Version:", version);
    form->addRow("Type:", type);
    form->addRow("Minimum-stability:", minimumStability);
    form->addRow("Prefer-stable:", preferStable);
    form->setRowWrapPolicy(QFormLayout::WrapAllRows);

    return form;
}

QFormLayout *General::createRightFormLayout()
{
    QLineEdit *keywords = new QLineEdit;
    QLineEdit *license = new QLineEdit;
    QLineEdit *readme = new QLineEdit;
    QLineEdit *homepage = new QLineEdit;
    QDateTimeEdit *time = new QDateTimeEdit;

    time->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    time->setDateTime(QDateTime::currentDateTime());

    m_mapper->addMapping(keywords, 5);
    m_mapper->addMapping(license, 6);
    m_mapper->addMapping(readme, 7);
    m_mapper->addMapping(homepage, 8);
    m_mapper->addMapping(time, 9);

    keywords->setPlaceholderText("word1, word2...");

    QFormLayout *form = new QFormLayout;

    form->addRow("Keywords:", keywords);
    form->addRow("License:", license);
    form->addRow("Readme:", readme);
    form->addRow("Homepage:", homepage);
    form->addRow("Time:", time);
    form->setRowWrapPolicy(QFormLayout::WrapAllRows);

    return form;
}

QFormLayout *General::createDescriptionFormLayout()
{
    QPlainTextEdit *description = new QPlainTextEdit;

    m_mapper->addMapping(description, 10);

    QFormLayout *form = new QFormLayout;
    form->addRow("Description:", description);
    form->setRowWrapPolicy(QFormLayout::WrapAllRows);

    return form;
}

} // form
} // package
} // composer
} // composer_gui
