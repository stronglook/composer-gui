#pragma once

#include <QWidget>
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include <QPlainTextEdit>

#include "composer/package/form/visitor/AcceptorInterface.h"
#include "composer/package/form/visitor/VisitorInterface.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

class General : public QWidget, visitor::AcceptorInterface
{
    Q_OBJECT
public:
    enum Field {NAME, VERSION, TYPE, MINIMUM_STABILITY, PREFER_STABLE,
                KEYWORDS, LICENSE, README, HOMEPAGE, TIME, DESCRIPTION};

    explicit General(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;
    QStandardItemModel *getMappingModel();

private:
    QDataWidgetMapper *m_mapper;

    QFormLayout *createLeftFormLayout();
    QFormLayout *createRightFormLayout();
    QFormLayout *createDescriptionFormLayout();
};

} // form
} // package
} // composer
} // composer_gui
