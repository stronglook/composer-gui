#pragma once

#include <QWidget>
#include <QFormLayout>
#include <QStandardItemModel>
#include <QDataWidgetMapper>

#include "composer/package/form/visitor/VisitorInterface.h"
#include "composer/package/form/visitor/AcceptorInterface.h"

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{

class Support : public QWidget, public visitor::AcceptorInterface
{
public:
    enum Field {EMAIL, ISSUES, FORUM, WIKI, IRC, SOURCE, DOCS, CHAT};

    Support(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;
    QStandardItemModel *getMappingModel();

private:
    QDataWidgetMapper *m_mapper;

    QFormLayout *createFormLayout();
};

} // form
} // package
} // composer
} // composer_gui
