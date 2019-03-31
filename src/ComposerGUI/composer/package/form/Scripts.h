#pragma once

#include <QWidget>
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

class Scripts : public QWidget, public visitor::AcceptorInterface
{
public:
    Scripts(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;
    QStandardItemModel *getMappingModel();

private:
    QStandardItemModel *m_model;
};

} // form
} // package
} // composer
} // composer_gui
