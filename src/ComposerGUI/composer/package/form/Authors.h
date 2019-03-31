#pragma once

#include <QWidget>
#include <QTableView>
#include <QStandardItemModel>

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

class Authors : public QWidget, public visitor::AcceptorInterface
{
public:
    enum Field {NAME, ROLE, EMAIL, HOMEPAGE};

    Authors(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;
    QStandardItemModel *getMappingModel();

private:
    QStandardItemModel *m_model;

    QTableView *createTableView();
    QStandardItemModel *createTableModel();
};

} // form
} // package
} // composer
} // composer_gui
