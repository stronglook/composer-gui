#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QModelIndex>
#include <QDataWidgetMapper>

namespace composer_gui
{
namespace composer
{
namespace package
{
namespace form
{
namespace delegate
{
namespace editor
{

class Dependency : public QDialog
{
public:
    Dependency(QWidget *parent = nullptr);
    void setModelIndex(const QModelIndex &index);

private:
    QLineEdit *m_package;
    QDataWidgetMapper *m_mapper;
};

} // editor
} // delegate
} // form
} // package
} // composer
} // composer_gui
