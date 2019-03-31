#pragma once

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QHash>

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

class Autoload : public QWidget, public visitor::AcceptorInterface
{
public:
    Autoload(QWidget *parent = nullptr);
    void accept(visitor::VisitorInterface &visitor) override;
    QStandardItemModel *getMappingModel(const QString &name);

private:
    QHash<QString, QStandardItemModel *> m_models;

    QTreeView *createView(const QString &name);
    QStandardItemModel *createModel(const QString &name);
};

} // form
} // package
} // composer
} // composer_gui
